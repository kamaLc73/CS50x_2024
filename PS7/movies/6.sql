SELECT avg(rating)
from ratings
where movie_id in
(
    SELECT id
    FROM movies
    where year = 2012
);
