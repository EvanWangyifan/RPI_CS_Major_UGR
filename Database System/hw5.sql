--Query 1 Return the name of all directors (other than Steven Spielberg) 
--o have the same number of movies as Steven Spielberg in the database. 
--Order by director.
SELECT
	c.drt as director
FROM
	(SELECT
		md.director as drt,
		count(*) as numtimes
	FROM
		moviesdirectors md,
		movies m
	WHERE
		m.movieid=md.movieid
	GROUP BY
		md.director) as c
WHERE
	c.drt<>'Steven Spielberg'
	AND c.numtimes=(SELECT count(*) 
					FROM moviesdirectors d,movies mv 
					WHERE d.movieid=mv.movieid and d.director='Steven Spielberg'
					GROUP BY d.director)
ORDER BY
	director;


--Query 2 For each country in the database (in countryseries), return 
--the country and the total number of series (numseries) they offer in 
--the TV Comedies category. Order results by num- series descending and 
--country ascending.
SELECT
	c.country,
	count(cg.category) as numseries
FROM
	(series s right join seriescountry c on s.seriesid=c.seriesid) 
		left join seriescategory cg on s.seriesid=cg.seriesid
						and cg.category='TV Comedies'
GROUP BY
	c.country
ORDER BY
	numseries desc,
	c.country asc;

--Query 3 Order languages by the number of movies that they have in 
--descending order and by language name in ascending order. Return the 
--languages that rank between 15-40 (inclusive at both ends) in this 
--ordering, order the results in ascending order of number of movies 
--and language ascending. (Note: this was a favorite interview query 
--for a while...)
SELECT
	a.language,
	a.num
FROM
	(SELECT
		ml.language as language,
		count(*) as num
	FROM
		movies m,
		movieslanguages ml
	WHERE
		m.movieid=ml.movieid
	GROUP BY
	    ml.language
	ORDER BY
		num desc,
		ml.language asc
	LIMIT
		26 offset 14) as a
ORDER BY
	a.num asc,
	a.language asc;


--Query 4 Find movies offered in the highest number of languages. Return 
--the title of the movie, number of languages (nummovies). Order results 
--by title ascending.
SELECT
	a.title,
	a.numlanguages
FROM
	(SELECT
		m.title as title,
		count(*) as numlanguages
	FROM
		movies m,
		movieslanguages l
	WHERE
		m.movieid=l.movieid
	GROUP BY
		m.title
	ORDER BY
		numlanguages desc) as a
WHERE
	a.numlanguages=(SELECT
						count(*) as n
					FROM
						movies m1,
						movieslanguages l1
					WHERE
						m1.movieid=l1.movieid
					GROUP BY
						m1.title
					ORDER BY
						n desc
					LIMIT
						1)
ORDER BY
	a.title asc;


--Query 5 Find the difference between the average imdbrating of movies in 
--the Horror genre re- leased before year 2000 and after year 2000. Return 
--the average rating before 2000 (avg- before), after 2000 (avgafter) and 
--the difference (avgdiff). Cast all your average values as NUMERIC(5,2). 
--Use the movies.year attribute in this query.
SELECT
	a.avgbefore,
	b.avgafter,
	(a.avgbefore-b.avgafter) as avgdiff
FROM
	(SELECT
		avg(m.imdbrating)::NUMERIC(5,2) as avgbefore
	FROM
		movies m,
		moviesgenres g
	WHERE
		m.movieid=g.movieid
		and g.genre='Horror'
		and m.year<2000) as a,
	(SELECT
		avg(m.imdbrating)::NUMERIC(5,2) as avgafter
	FROM
		movies m,
		moviesgenres g
	WHERE
		m.movieid=g.movieid
		and g.genre='Horror'
		and m.year>2000) as b;


--Query 6 For the following countries: Turkey, France, China, India, Thailand, 
--Japan, find series with an imdb rating of at least 7.5 and are only available 
--in that country. Return the country name, series title and its imdbrating. 
--Order results by country, imdbrating and series title ascending.
SELECT
	c.country as country,
	s.title as title,
	s.imdbrating as imdbrating
FROM
	series s,
	seriescountry c
WHERE
	s.seriesid=c.seriesid
	and c.country in ('Turkey', 'France', 'China', 'India', 'Thailand', 'Japan')
	and s.imdbrating>=7.5

EXCEPT

SELECT
	c1.country as country,
	s.title as title,
	s.imdbrating as imdbrating
FROM
	series s,
	seriescountry c1,
	seriescountry c2
WHERE
	s.seriesid=c1.seriesid
	and s.seriesid=c2.seriesid
	and c1.country in ('Turkey', 'France', 'China', 'India', 'Thailand', 'Japan')
	and c1.country<>c2.country
	and s.imdbrating>=7.5

ORDER BY
	country,
	imdbrating,
	title;


--Query 7 This query builds on the logic of the previous query. For each country 
--(other than United States), find series that are only available in that country. 
--Among these series, find the series with the highest imdbrating for each country. 
--Return the country, title and the imdb rating of these series, order by country 
--and title asc.
SELECT 
	a.country,
	a.title,
	a.imdbrating
FROM
	(SELECT
		c.country as country,
		s.title as title,
		s.imdbrating as imdbrating
	FROM
		series s,
		seriescountry c
	WHERE
		s.seriesid=c.seriesid
		and c.country<>'United States'
	EXCEPT
	SELECT
		c1.country as country,
		s.title as title,
		s.imdbrating as imdbrating
	FROM
		series s,
		seriescountry c1,
		seriescountry c2
	WHERE
		s.seriesid=c1.seriesid
		and s.seriesid=c2.seriesid
		and c1.country<>'United States'
		and c1.country<>c2.country
	ORDER BY
		country,
		title) as a
WHERE
	a.imdbrating=(	SELECT max(b.imdbrating1)
					FROM 
						(SELECT
							c3.country as country1,
							s3.imdbrating as imdbrating1
						FROM
							series s3,
							seriescountry c3
						WHERE
							s3.seriesid=c3.seriesid
							and c3.country<>'United States'
						EXCEPT
						SELECT
							c4.country as country1,
							s4.imdbrating as imdbrating1
						FROM
							series s4,
							seriescountry c4,
							seriescountry c5
						WHERE
							s4.seriesid=c4.seriesid
							and s4.seriesid=c5.seriesid
							and c4.country<>'United States'
							and c4.country<>c5.country) as b
					WHERE a.country=b.country1)
ORDER BY
	a.country,
	a.title;


--Query 8 Find pairs of platforms (renamed platform1, platform2) such that platform2 
--offers more than 60% of the series offered by platform1. Return pairs of platforms 
--(renamed platform1, platform2) and proportion of series they have in common 
--(numcommon) formatted as nu- meric(5,2). Order the results by platform1, platform2 
--and numcommon.
SELECT DISTINCT
	*
FROM
	(SELECT
		p1.platform as platform1,
		p2.platform as platform2,
		(b.n2*1.0/a.n1)::numeric(5,2) as numcommon
	FROM
		series s,
		seriesonplatform p1,
		seriesonplatform p2,
		(SELECT
			p3.platform as pf1,
			count(*) as n1
		FROM
			series s1,
			seriesonplatform p3
		WHERE
			s1.seriesid=p3.seriesid
		GROUP BY
			p3.platform) as a,
		(SELECT
			p4.platform as pf2,
			p5.platform as pf3,
			count(*) as n2
		FROM
			series s2,
			seriesonplatform p4,
			seriesonplatform p5
		WHERE
		 	s2.seriesid=p4.seriesid
			and s2.seriesid=p5.seriesid
			and p4.seriesid=p5.seriesid
		GROUP BY
			p4.platform,p5.platform) as b
	WHERE
		s.seriesid=p1.seriesid
	    and s.seriesid=p2.seriesid
		and p1.platform<>p2.platform
		and a.pf1=p1.platform
		and b.pf2=p1.platform
		and b.pf3=p2.platform) as ratio
WHERE
	ratio.numcommon>0.60
ORDER BY
	ratio.platform1,
	ratio.platform2;


--Query 9 Return the name of cast members who have starred in more than two of the 
--same director’s movies and did not direct any movies themselves. Order results by 
--cast member name ascending.
SELECT *
FROM
(SELECT
    c.castname
FROM
    moviescast c, moviesdirectors d
WHERE
    c.movieid = d.movieid
GROUP BY
    c.castname, d.director
HAVING
    count(*)>2

EXCEPT

SELECT
    mc.castname
FROM
    moviescast mc, moviesdirectors md
WHERE
    mc.castname = md.director
) as a

ORDER BY
    castname asc;






