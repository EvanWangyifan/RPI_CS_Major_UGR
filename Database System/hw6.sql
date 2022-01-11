
CREATE OR REPLACE FUNCTION
    recommendation(inputseries integer array, topk int
	       			, w1 float, w2 float, w3 float, w4 float)
    RETURNS varchar AS $$
DECLARE
   values VARCHAR ;
   myrow RECORD ;
BEGIN
values='';
   FOR myrow IN 
   		SELECT 
   			sr.title as title,
   			t1.numtimes as a1,
   			t2.numtimes as a2,
   			t3.numtimes as a3,
   			t4.score as a4
        FROM 
        	(--a1
			SELECT
				s.seriesid as id,
				count(*) as numtimes
			FROM
				series s,
				seriescategory c1,
				seriescategory c2,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid=c1.seriesid
				and c2.seriesid=input.seriesid 
				and s.seriesid<>input.seriesid 
				and c1.category=c2.category
			GROUP BY
				s.seriesid) as t1,

        	(--a2
			SELECT
				s.seriesid as id,
				count(distinct p.platform) as numtimes
			FROM
				series s,
				seriesonplatform p,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid=p.seriesid
				and s.seriesid<>input.seriesid
			GROUP BY
				s.seriesid) as t2,

        	(--a3
			SELECT
				s.seriesid as id,
				count(*) as numtimes
			FROM
				series s,
				seriescast c1,
				seriescast c2,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid=c1.seriesid
				and s.seriesid<>input.seriesid
				and c2.seriesid=input.seriesid
				and c1.castname=c2.castname
			GROUP BY
				s.seriesid
			UNION
			SELECT
				s.seriesid as id,
				count(*) as numtimes
			FROM
				series s,
				seriesdirectors d1,
				seriesdirectors d2,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid=d1.seriesid
				and s.seriesid<>input.seriesid
				and d2.seriesid=input.seriesid
				and d1.director=d2.director
			GROUP BY
				s.seriesid) as t3,

        	(--a4
			SELECT
				s.seriesid as id,
				0 as score
			FROM
				series s,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid<>input.seriesid
				and s.imdbrating is null
				and s.rottentomatoes is null
			UNION

			SELECT
				s.seriesid as id,
				s.imdbrating as score
			FROM
				series s,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid<>input.seriesid
				and s.imdbrating is not null
				and s.rottentomatoes is null
			UNION

			SELECT
				s.seriesid as id,
				s.rottentomatoes/10::NUMERIC(6,3) as score
			FROM
				series s,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid<>input.seriesid
				and s.imdbrating is null
				and s.rottentomatoes is not null
			UNION

			SELECT
				s.seriesid as id,
				((s.rottentomatoes/10::NUMERIC(6,3)+s.imdbrating)/2)::NUMERIC(6,3) as score
			FROM
				series s,
				unnest(inputseries) as input(seriesid)
			WHERE
				s.seriesid<>input.seriesid
				and s.imdbrating is not null
				and s.rottentomatoes is not null) as t4,

        	series sr,
        	unnest(inputseries) as input(seriesid)
        WHERE  
        	sr.seriesid<>input.seriesid
        	and sr.seriesid=t1.id
        	and sr.seriesid=t2.id
        	and sr.seriesid=t3.id
        	and sr.seriesid=t4.id
        ORDER BY
        	t1.numtimes*w1+t2.numtimes*w2+t3.numtimes*w3+t4.score*w4 desc,
        	title asc
        LIMIT
        	topk
   LOOP
       values = values || myrow.title || ' (' || 
       			(myrow.a1*w1+myrow.a2*w2+myrow.a3*w3+myrow.a4*w4)::NUMERIC(6,3) 
       			|| ')' || E'\n'; 
   END LOOP ;
   
   RETURN values ;
END ;
$$ LANGUAGE plpgsql ;


