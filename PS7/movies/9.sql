select distinct name
from people
join stars on people.id = stars.person_id
join movies on stars.movie_id = movies.id
where movies.year = 2004
order by people.birth asc;
