-- Keep a log of any SQL queries you execute as you solve the mystery.

-- View all tables
.tables
.schema

-- View crime reports for the day
SELECT *
FROM crime_scene_reports
WHERE year = 2023
AND month = 7
AND day = 28
AND street = "Humphrey Street";

-- View interviews with three eyewitnesses about the thief
SELECT *
FROM interviews
WHERE year = 2023
AND month = 7
AND day = 28
AND transcript LIKE "%thief%" ;

-- Transcript 1: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
SELECT *
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 15
    AND minute <= 25
    AND activity = "exit"
)
ORDER BY name;
-- People leaving the parking lot
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
-- +--------+---------+----------------+-----------------+---------------+



-- Transcript 2: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
-- I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
SELECT *
FROM people JOIN bank_accounts
ON people.id = bank_accounts.person_id
WHERE account_number IN
(
    SELECT account_number
    FROM atm_transactions
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND atm_location = "Leggett Street"
    AND transaction_type = "withdraw"
)
ORDER BY name;
-- People who withdrew money on Leggett Street that day
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       | 81061156       | 438727    | 2018          |
-- | 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       | 16153065       | 458378    | 2012          |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          |
-- | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       | 396669    | 2014          |
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | 28296815       | 395717    | 2014          |
-- | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | 467400    | 2014          |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       | 449774    | 2015          |
-- +--------+---------+----------------+-----------------+---------------+----------------+-----------+---------------+



-- Transcript 3: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief
-- say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
SELECT *
FROM people
WHERE passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id IN
    (
        SELECT id
        FROM flights
        WHERE year = 2023
        AND month = 7
        AND day = 29
        AND origin_airport_id IN
        (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville"
        )
            ORDER BY hour
            LIMIT 1
    )
)
ORDER BY name;
-- Person purchasing the earliest flight ticket for the next day
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- +--------+--------+----------------+-----------------+---------------+



SELECT *
FROM people
WHERE phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
)
ORDER BY name;
-- People who made phone calls lasting less than a minute that day
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 907148 | Carina  | (031) 555-6622 | 9628244268      | Q12B3Z3       |
-- | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- +--------+---------+----------------+-----------------+---------------+

--So bruce is highly likely that he is the thief.
--Lets find the the accomplice

SELECT *
FROM people
WHERE phone_number in
(
    SELECT receiver
    FROM phone_calls
    WHERE caller = "(367) 555-5533"
    AND year = 2023
    AND month = 7
    AND day = 28
    AND duration < 60
);
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+

--So Robin is the accomplice

--Finding what city Bruce escaped to
SELECT a.city
FROM airports AS a
JOIN flights AS f ON a.id = f.destination_airport_id
WHERE f.id IN
(
    SELECT flight_id
    FROM passengers
    WHERE passport_number = 5773159633
);

-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+

--So he escaped to New York City
