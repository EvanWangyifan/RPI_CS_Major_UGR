
--Query 1 Return the title and directors of series with at least 15
--seasons despite having an IMDB rating of less than 5. 
--Order results by title and director ascending
SELECT 'Query 1';
SELECT
	s.title,
	sd.director
FROM
	series s,
	seriesdirectors sd
WHERE
	s.seriesid=sd.seriesid
	and s.seasons>=15
	and s.imdbrating<5
ORDER BY
	s.title,
	sd.director;


--Query 2 Find the number of movies (nummovies) that have no 
--imdb or rotten tomatoes rating, and either has no year value 
--to year is after 2015.
SELECT 'Query 2';
SELECT
	count(*) as nummovies
FROM
	movies m
WHERE
	m.imdbrating IS NULL
	and m.rottentomatoes IS NULL
	and (m.year IS NULL or m.year>2015);


--Query 3 Return the title, director of all movies IMDB rating 
--of 8 or higher and is available in English and in one other language, 
--and are in at least one of the following genres: Action, Sci-Fi, 
--Adventure or Drama. Order results by title and director ascending.
SELECT 'Query 3';
SELECT DISTINCT
	m.title,
	md.director
FROM
	movies m,
	moviesgenres mg,
	movieslanguages ml1,
	movieslanguages ml2,
	moviesdirectors md
WHERE
	m.movieid=mg.movieid
	and m.movieid=md.movieid
	and m.movieid=ml1.movieid
    and m.movieid=ml2.movieid
	and m.imdbrating>=8
	and lower(ml1.language)='english' 
	and lower(ml1.language)<>lower(ml2.language)
	and mg.genre IN ('Action', 'Sci-Fi', 'Adventure', 'Drama')
ORDER BY
	m.title,
	md.director;


--Query 4 Return the id, title of all movies and TVshows that 
--are only available in Italy. Return an additional column 
--called streamingtype that has the value ’movie’ or ’series’ 
--depending on the source. Order results by the title and streamingtype 
--in descending order.
SELECT 'Query 4';
(SELECT
	m.movieid as id,
	m.title as title,
	'movie' as streamingtype
FROM
	movies m,
	moviescountry mc
WHERE
	m.movieid=mc.movieid
	and mc.country='Italy'
UNION
SELECT 
	s.seriesid as id,
	s.title as title,
	'series' as streamingtype
FROM
	series s,
	seriescountry sc
WHERE
    s.seriesid=sc.seriesid
	and sc.country='Italy')

EXCEPT

(SELECT
	m.movieid as id,
	m.title as title,
	'movie' as streamingtype
FROM
	movies m,
	moviescountry mc1,
	moviescountry mc2
WHERE
	m.movieid=mc1.movieid
	and m.movieid=mc2.movieid
	and mc1.country='Italy'
	and mc2.country<>mc1.country
UNION
SELECT 
	s.seriesid as id,
	s.title as title,
	'series' as streamingtype
FROM
	series s,
	seriescountry sc1,
	seriescountry sc2
WHERE
    s.seriesid=sc1.seriesid
    and s.seriesid=sc2.seriesid
	and sc1.country='Italy'
	and sc2.country<>sc1.country)

ORDER BY
	title DESC,
	streamingtype DESC;


--Query 5 Return the total number of tuples in the movies relation 
--(numtuples), total number of tuples with a date added value (numdate), 
--earliest year and last year for a movie (minyear, maxyear), 
--average duration for a movie and average imdb rating (avgrating).
SELECT 'Query 5';
SELECT DISTINCT *
FROM
	(SELECT
		count(m.movieid) as numtuples
	FROM
		movies m
	) as a,
	
	(SELECT
		count(*) as numdate
	FROM
		movies m
	WHERE
		m.date_added IS NOT NULL
	) as b,
	
	(SELECT min(year) as minyear FROM movies)
	 as c,
	(SELECT max(year) FROM movies)
	 as d,

	(SELECT
		AVG(RTRIM(duration, 'min')::int)::numeric(5,2) as avgduration
	FROM 
		movies movies
	) as e,

	(SELECT
		cast(AVG(m.imdbrating) as decimal(16,2)) as avgrating
	FROM	
		movies m
	) as f;

--Query 6 Find cast members who have both been in a movie and a 
--TV Series in the same genre for either Mystery or Thriller genres. 
--Return the name of the cast members and the total number of movies 
--(nummovies) and series (numseries) they have been in. Order by the 
--nummovies, numseries and castname ascending.
SELECT 'Query 6';
SELECT 
	mc.castname as castname,
	count(DISTINCT m.movieid) as nummovies,
	count(DISTINCT s.seriesid) as numseries
FROM
	movies m,
	moviesgenres mg,
	moviescast mc,
	series s,
	seriesgenres sg,
	seriescast sc
WHERE
	m.movieid=mg.movieid
	and m.movieid=mc.movieid
	and s.seriesid=sg.seriesid
	and s.seriesid=sc.seriesid
	and mc.castname=sc.castname
	and mg.genre=sg.genre 
	and (mg.genre='Mystery' or mg.genre='Thriller')
GROUP BY
	mc.castname
ORDER BY
	nummovies, 
	numseries,
	castname;


--Query 7 Find series that are available in at least 3 countries. 
--Return the title of these series, the total number countries 
--(numcountries) and streaming platforms they are available in. 
--Order results by numcountries descending and title ascending.
SELECT 'Query 7';
SELECT
	s.title as title,
	count(DISTINCT sc1.country) as numcountries,
	count(DISTINCT sp.platform) as numplatforms
FROM
	series s,
	seriescountry sc1,
	seriescountry sc2,
	seriescountry sc3,
	seriesonplatform sp
WHERE
	s.seriesid=sc1.seriesid
	and s.seriesid=sc2.seriesid
	and s.seriesid=sc3.seriesid
	and s.seriesid=sp.seriesid
	and sc1.country<>sc2.country
	and sc1.country<>sc3.country
	and sc2.country<>sc3.country
GROUP BY
	s.title
ORDER BY
	numcountries DESC,
	title;


--Query 8 For each language, find how many movies are in that 
--language. Return the language, min and max imdb and rotten tomates 
--ratings of series for each langugage that has at least 10 movies.
--Rename the aggregates: minimdb, maximdb, minrotten, maxrotten 
--respectively. Order results by minimdb and minrotten asc.
SELECT 'Query 8';
SELECT
  	ml.language as language, 
    min(m.imdbrating) as minimdb, 
    max(m.imdbrating) as maximdb, 
    min(m.rottentomatoes) as minrotten, 
    max(m.rottentomatoes) as maxrotten
FROM
    movies m,
    movieslanguages ml
WHERE
  	m.movieid = ml.movieid
GROUP BY
  	ml.language
HAVING
  	count(ml.movieid) >=10
ORDER BY
  	minimdb, 
    minrotten;


--Query 9 For series that are on fuboTV, find the other platforms 
--that they are also on. For each other platform, return the total 
--number of series from fuboTV that they feature (numseries). 
--Order results by numseries.
SELECT 'Query 9';
SELECT DISTINCT
	p.platform,
	count(p.seriesid) as numseries
FROM
	(SELECT
		s.seriesid as id
	FROM 
		series s,
		seriesonplatform sp
	WHERE
		s.seriesid=sp.seriesid
		and sp.platform='fuboTV') as sid,
	seriesonplatform p
WHERE
	sid.id=p.seriesid
	and p.platform<>'fuboTV'
GROUP BY
	p.platform
ORDER BY
	numseries DESC;


--Query 10 Return the name of all cast members who were cast in a 
--movie added after 1/1/2019 and have also directed a movie but were 
--not cast in any series. Order the results by castname ascending.
SELECT 'Query 10';
SELECT DISTINCT
	mc.castname as castname
FROM
	movies m,
	moviescast mc,
	moviesdirectors md
WHERE
	m.movieid=mc.movieid
	and mc.castname=md.director
	and m.date_added>date '2019-01-01'
EXCEPT
SELECT
 	sc.castname
FROM
 	seriescast sc

ORDER BY
    castname;




