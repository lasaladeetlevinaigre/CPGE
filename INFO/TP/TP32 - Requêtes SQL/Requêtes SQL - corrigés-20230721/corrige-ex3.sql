

-- question 1: les différentes attributs d'une table ont un 
-- type (VARCHAR, NUMBER, NULL...) mais peuvent également 
-- avoir un domaine d'existence, sorte de précondition pour
-- qu'un enregsitrement soit valide. La clause CONSTRAINT 
-- permet d'attribuer un identificateur à une contrainte

-- question 2.a

SELECT c.Name 
FROM Country  c
WHERE c.Population >= 60000000;

-- question 2.b

SELECT c.Name 
FROM Country  c
WHERE c.Population >= 60000000
ORDER BY c.Name ASC;

-- question 2.c

SELECT c.Name 
FROM Country  c
WHERE c.Population >= 60000000
ORDER BY c.Population DESC;

-- question 2.d

SELECT c.Name 
FROM Country  c
ORDER BY c.Area ASC
LIMIT 10;

-- question 2.e

SELECT c.Name 
FROM Country  c
ORDER BY c.Area ASC
LIMIT 10
OFFSET 10;


-- question 3.a

-- Deux solutions proposées.

-- Sans utiliser le pourcentage mais en comptant le nombre de 
-- continents différents auxquels un pays est associé 

SELECT c.Name 
FROM Country c  
JOIN encompasses e 
ON c.Code = e.Country
WHERE 2 <= (
SELECT COUNT(*)
FROM encompasses e
WHERE c.Code = e.Country
GROUP BY e.Country
)


-- en utilisant le pourcentage du pays sur un contient. Si 
-- < 100%, le pays est à cheval sur plusieurs continents.

SELECT c.Name 
FROM Country c  
JOIN encompasses e 
ON c.Code = e.Country
WHERE e.Percentage < 100;

-- question 3.b

SELECT c.Name 
FROM Country c
JOIN encompasses e
JOIN Continent cont
ON e.Country = c.Code AND cont.Name = e.Continent
WHERE e.Continent = 'America' AND (CAST(c.Population AS real)/ CAST(c.Area AS real) < 10);

-- question 3.c

SELECT cit.Name 
FROM City cit
JOIN Country c
JOIN encompasses e
JOIN Continent cont
ON e.Country = c.Code AND cont.Name = e.Continent AND cit.Name = c.Capital
WHERE e.Continent = 'Europe' AND cit.Latitude >= 60;

-- question 4.a

SELECT l.Name AS 'Langue', COUNT(l.Country) AS 'Nombre de pays ou cette langue est parlee'
FROM Language l
JOIN Country c
ON l.Country = c.Code
GROUP BY l.Name
;


SELECT l.Name AS 'Langue', COUNT(l.Country) AS 'Nombre de pays ou cette langue est parlee'
FROM Language l
JOIN Country c
ON l.Country = c.Code
GROUP BY l.Name
ORDER BY COUNT(l.Country) DESC
LIMIT 10
;


-- question 4.b

SELECT l.Name AS 'Langue', CAST (SUM(CAST(l.Percentage AS real)/100. * CAST(c.Population AS real)) AS int) AS 'Nombre de personnes parlant cette langue'
FROM Language l
JOIN Country c
ON l.Country = c.Code
GROUP BY l.Name
ORDER BY CAST (SUM(CAST(l.Percentage AS real)/100. * CAST(c.Population AS real)) AS int) DESC
LIMIT 5
;

-- On peut s'étonner de l'absence de la langue chinoise!
-- en fait, cette langue n'est pas bien répertoriée dans cette
-- base comme le montre les requetes suivantes:

SELECT c.Name
FROM Country c
WHERE c.Code = 'CN'; -- montre que le pays Chine est bien là

SELECT l.Name 
FROM Language l
JOIN Country c
ON l.Country = c.Code
WHERE c.Code = 'CN'
;  -- montre qu'aucune ligne de la table d'association Langage
-- ne mentionne que le chinois est parlé... par les 1 milliards d'habitants de la Chine!


-- question 4.c

-- on peut deja regarder le pourcentage de gens parlant
-- francis dans les pays ou il est parlé
SELECT c.Name, l.Percentage
FROM Language l
JOIN Country c
ON l.Country = c.Code
WHERE l.Name = 'French'
;


SELECT c.Name 
FROM Language l
JOIN Country c
ON l.Country = c.Code
WHERE l.Name = 'French' AND l.Percentage < 100 AND 
l.Percentage = 
(
    SELECT MIN(l.Percentage)
    FROM Language l
    WHERE l.Name = 'French'
    GROUP BY l.Country
)
;


-- On peut lister les pourcentages des toutes les langues parlées dans les pays ou l'on parle le français

SELECT c.Name, l.Name, l.Percentage
FROM Country c
JOIN Language l
JOIN (SELECT l.Country
FROM Language l
WHERE l.Name = 'French'
) f
ON c.Code = l.Country AND l.Country = f.Country
;


SELECT c.Name, l.Name, l.Percentage
FROM Country c
JOIN Language l
JOIN (
        SELECT l.Country, l.Percentage
        FROM Language l
        WHERE l.Name = 'French'
    ) f
ON c.Code = l.Country AND l.Country = f.Country
WHERE l.Percentage = 
    (
        SELECT MIN(f2.Percentage) 
        FROM 
        (
            SELECT l.Country, l.Percentage
            FROM Language l
            WHERE l.Name = 'French'
        ) f2
        GROUP BY f2.Country
    ) AND l.Name = 'French'
;

-- On peut remarquer que le total les poucentages de toutes les langues associées à un pays ne fait pas toujours 100%
SELECT c.Name, SUM(l.Percentage)
    FROM Language l
    JOIN Country c
    ON c.Code = l.Country
    GROUP BY l.Country;

