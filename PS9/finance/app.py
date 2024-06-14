import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

#######################################################


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

#######################################################


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    try:
        user_id = session["user_id"]

        # Fetch stocks owned by the user
        stocks = db.execute(
            "SELECT symbol, SUM(shares) as total_shares FROM orders WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
            user_id
        )

        # Fetch the user's cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        total_value = 0
        grand_total = cash

        # Update stock information with current price and value
        for stock in stocks:
            quote = lookup(str(stock["symbol"]))
            if quote is None:
                return apology("Lookup failed", 400)
            stock["price"] = quote["price"]
            stock["value"] = quote["price"] * stock["total_shares"]
            total_value += stock["value"]
            grand_total += stock["value"]

        return render_template("index.html", stocks=stocks, cash=cash, total_value=total_value, grand_total=grand_total)

    except Exception as e:
        # Log the error for debugging
        print(f"Error: {e}")
        return apology("An error occurred while retrieving your portfolio", 500)


#######################################################

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Validate symbol
        if not symbol:
            return apology("must provide a symbol")

        # Validate shares
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("must provide a positive integer number of shares")

        shares = int(shares)

        # Lookup the stock quote
        quote = lookup(symbol)
        if quote is None:
            return apology("symbol not found")

        # Retrieve user's cash balance
        user_id = session["user_id"]

        price = quote["price"]
        total_cost = int(shares) * price
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Check if the user has enough cash
        if cash < total_cost:
            return apology("not enough cash")

        # Update user's cash balance
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, user_id)

        # Record the transaction
        db.execute("INSERT INTO orders (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   user_id, symbol, shares, price)

        # Provide feedback to the user
        flash(f"Bought {shares} shares of {symbol} for {usd(total_cost)}!")
        return redirect("/")
    elif request.method == "GET":
        return render_template("buy.html")

#######################################################


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:
        user_id = session["user_id"]

        # Fetch the user's transaction history
        transactions = db.execute(
            "SELECT symbol, shares, price, timestamp FROM orders WHERE user_id = ? ORDER BY timestamp DESC",
            user_id
        )

        if not transactions:
            return apology("No transaction history available", 400)

        return render_template("history.html", transactions=transactions)
    except Exception as e:
        # Log the error for debugging
        print(f"Error: {e}")
        return apology("An error occurred while retrieving your History transaction", 500)


#######################################################


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

#######################################################


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

#######################################################


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)
        return render_template("quote.html", quote=quote)
    else:
        return render_template("quote.html")

#######################################################


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username or not password or not confirmation:
            return apology("Please provide username and password!")
        if password != confirmation:
            return apology("Password and confirmation don't match")
        if db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("User already exists!", 400)

        # Insert new user into the database
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username, generate_password_hash(password),
        )

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        session["user_id"] = rows[0]["id"]
        return redirect("/")

#######################################################


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        user_id = session["user_id"]
        symbol = request.form.get("symbol").upper()
        try:
            shares_to_sell = int(request.form.get("shares"))
        except (ValueError, TypeError):
            return apology("must provide a valid number of shares", 400)

        if not symbol:
            return apology("must provide symbol", 400)
        if shares_to_sell <= 0:
            return apology("must provide positive number of shares", 400)

        # Check if user owns the stock and has enough shares
        rows = db.execute(
            "SELECT SUM(shares) as total_shares FROM orders WHERE user_id = ? AND symbol = ? GROUP BY symbol",
            user_id, symbol
        )

        if not rows or rows[0]["total_shares"] < shares_to_sell:
            return apology("not enough shares", 400)

        # Lookup stock price
        quote = lookup(symbol)
        if not quote:
            return apology("symbol not found", 400)

        # Calculate the total sale value
        sale_price = quote["price"]
        total_sale_value = shares_to_sell * sale_price

        # Update user's cash balance
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", total_sale_value, user_id)

        # Record the transaction (negative shares indicate selling)
        db.execute("INSERT INTO orders (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   user_id, symbol, -shares_to_sell, sale_price)

        # Provide feedback to the user
        flash(f"Sold {shares_to_sell} shares of {symbol} for {usd(total_sale_value)}!")
        return redirect("/")

    else:
        """Sell shares of stock"""
        user_id = session["user_id"]

        # Fetch stocks owned by the user
        stocks = db.execute(
            "SELECT symbol, SUM(shares) as total_shares FROM orders WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0",
            user_id
        )

        if not stocks:
            return apology("no stocks available to sell", 400)

        # Update stock information with current price and value
        for stock in stocks:
            quote = lookup(stock["symbol"])
            if not quote:
                return apology(f"lookup failed for {stock['symbol']}", 400)
            stock["price"] = quote["price"]
            stock["value"] = stock["total_shares"] * quote["price"]

        return render_template("sell.html", stocks=stocks)
