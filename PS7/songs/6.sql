select name
from songs
where artist_id in
(
    SELECT id
    from artists
    where name = 'Post Malone'
);
