--- CORRIGE TP 32 --

-- question 2 -- 
SELECT C.nom_commune, D.nom_departement
FROM communes C
JOIN departements D
ON C.id_departement = D.id_departement;


SELECT nom_commune, population
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE nom_region = "Limousin" 
AND population < 
(	SELECT AVG(population)
	FROM communes c
	JOIN regions r
	ON c.id_region = r.id_region
	WHERE nom_region = "Limousin")
;



-- question 2 -- 
SELECT nom_commune, population, nom_departement, nom_region
FROM communes C
JOIN departements D
ON C.id_departement = D.id_departement
JOIN regions R
ON C.id_region = R.id_region;


-- question 3 --

SELECT nom_commune, population, nom_region
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE c.population >= 100000;

-- question 4 --

SELECT nom_commune, population, nom_region
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE c.population >= 100000
ORDER BY population ASC;


SELECT nom_commune, population, nom_region
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE c.population >= 100000
ORDER BY population DESC;


-- question 5a: Limousin , code 74 --

SELECT COUNT(nom_commune)
FROM communes c
WHERE id_region = 74;

-- Inconvénient: on doit aller chercher à la main 
-- la clé primaire correspondant a la région Limousin pour
-- les enregistrements de communes qui contient cette clé 74
-- comme clé étrangère

-- question 5b:  avec jointure --

SELECT c.nom_commune
FROM communes c
JOIN regions r
ON r.id_region = c.id_region
WHERE nom_region = "Limousin";

-- question 5c
-- Sans le ON, la jointure effectue juste un 
-- produit cartésien, sans filtrage de jointure. 
-- Cela revient à faire un produit cartésien entre la table 
-- des communes, sans aucune contrainte, et celle des régions 
-- où l'on a selectionné que l'enregistrement correspondant au 
-- Limousin. 

-- question 5d
-- SELECT * sélectionne tous les attributs des deux tables. 
-- On a donc une colonne en double car id_region est présent 
-- dans les deux tables par exemple

SELECT *
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE nom_region = "Limousin";





-- question 6.b
-- NATURAL JOIN fonctionne par homonymie des attributs de deux 
-- tables différentes: 
-- la jointure est réalisée en filtrant les enregistrements 
-- pour ne garder que ceux qui ont la même valeur pour les 
-- attributs qui ont le même nom dasbs les deux tables. Ici, 
-- id_region a le même nom dans communes et dans regions 

-- Cette requete compte le nombre de communes de la région
-- Limousin.

SELECT COUNT(c.nom_commune)
FROM communes c
NATURAL JOIN regions
WHERE nom_region = "Limousin";


-- question 6.c Si SELECT *, on a tous les attributs de toutes
--  les tables
SELECT *
FROM communes c
NATURAL JOIN regions
WHERE nom_region = "Limousin";


-- question 6.d --
SELECT nom_commune, population
FROM communes c
NATURAL JOIN regions
WHERE nom_region = "Aquitaine";


SELECT nom_commune, population
FROM communes c
NATURAL JOIN regions
WHERE nom_region = "Aquitaine"
ORDER BY c.population ASC;


SELECT nom_commune, population
FROM communes c
NATURAL JOIN regions
WHERE nom_region = "Aquitaine"
ORDER BY c.population DESC;


-- Exercice 2

-- question 1 --

SELECT AVG(population)
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE nom_region = "Limousin";



-- question 2-- 
-- Cette requete calcule, pour chaque region, la population 
-- moyenne de ses communes et sa population totale
-- la clause GROUP BY sert a  savoir sur quels sous ensembles
-- sont effectuées les moyennes 

SELECT id_region, AVG(population), SUM(population)
FROM communes
GROUP BY id_region;


-- question 3 --

SELECT R.nom_region, AVG(population), SUM(population)
FROM regions R
JOIN communes C
ON c.id_region = r.id_region
GROUP BY r.id_region;


-- question 5 --

SELECT nom_commune, population
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE nom_region = "Limousin" 
AND population < 
(	SELECT AVG(population)
	FROM communes c
	JOIN regions r
	ON c.id_region = r.id_region
	WHERE nom_region = "Limousin");

	
-- question 5.5 : on écrit une requête imbriquée 

SELECT ROUND
(
    CAST(COUNT(nom_commune) AS FLOAT) 
    / 
    (
        SELECT COUNT(nom_commune)
        FROM communes c
        JOIN regions r
        ON c.id_region=r.id_region
        WHERE r.nom_region = "Limousin"
    ), 
2)
FROM communes c
JOIN regions r
ON c.id_region=r.id_region
WHERE r.nom_region = "Limousin"
AND population < 
(
    SELECT AVG(population)
    FROM communes c
    JOIN regions r
    ON c.id_region = r.id_region
    WHERE r.nom_region = "Limousin"
);

-- question 6

SELECT R.nom_region, COUNT(C.nom_commune) 
FROM regions R 
JOIN communes C ON R.id_region = C.id_region 
GROUP BY R.id_region 
HAVING COUNT(C.nom_commune) > 2000;


-- question 7 --
-- La clause AS a deux role différents dans cette
-- requete: un role dans le CAST pour forcer le type 
-- population a être un flottant
-- Un role dans les attributs récupérés, pour renommer les 
-- colonnes avec un nom choii (ici "Commune" "Population" et
-- "Pourcentage") dans l'affichage des résultats

SELECT nom_commune AS "Commune", population AS "Population",
ROUND
(
    100*CAST(population AS real)
    /
    (
        SELECT SUM(population)
        FROM communes NATURAL JOIN regions
        WHERE nom_region = "Limousin"
    ),
    2
) AS "Pourcentage"
FROM communes NATURAL JOIN regions
WHERE nom_region = "Limousin"
ORDER BY POPULATION DESC;

