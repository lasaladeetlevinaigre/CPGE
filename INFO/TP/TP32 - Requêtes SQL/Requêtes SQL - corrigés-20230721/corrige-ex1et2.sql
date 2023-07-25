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

-- Inconv�nient: on doit aller chercher � la main 
-- la cl� primaire correspondant a la r�gion Limousin pour
-- les enregistrements de communes qui contient cette cl� 74
-- comme cl� �trang�re

-- question 5b:  avec jointure --

SELECT c.nom_commune
FROM communes c
JOIN regions r
ON r.id_region = c.id_region
WHERE nom_region = "Limousin";

-- question 5c
-- Sans le ON, la jointure effectue juste un 
-- produit cart�sien, sans filtrage de jointure. 
-- Cela revient � faire un produit cart�sien entre la table 
-- des communes, sans aucune contrainte, et celle des r�gions 
-- o� l'on a selectionn� que l'enregistrement correspondant au 
-- Limousin. 

-- question 5d
-- SELECT * s�lectionne tous les attributs des deux tables. 
-- On a donc une colonne en double car id_region est pr�sent 
-- dans les deux tables par exemple

SELECT *
FROM communes c
JOIN regions r
ON c.id_region = r.id_region
WHERE nom_region = "Limousin";





-- question 6.b
-- NATURAL JOIN fonctionne par homonymie des attributs de deux 
-- tables diff�rentes: 
-- la jointure est r�alis�e en filtrant les enregistrements 
-- pour ne garder que ceux qui ont la m�me valeur pour les 
-- attributs qui ont le m�me nom dasbs les deux tables. Ici, 
-- id_region a le m�me nom dans communes et dans regions 

-- Cette requete compte le nombre de communes de la r�gion
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
-- la clause GROUP BY sert a� savoir sur quels sous ensembles
-- sont effectu�es les moyennes 

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

	
-- question 5.5 : on �crit une requ�te imbriqu�e 

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
-- La clause AS a deux role diff�rents dans cette
-- requete: un role dans le CAST pour forcer le type 
-- population a �tre un flottant
-- Un role dans les attributs r�cup�r�s, pour renommer les 
-- colonnes avec un nom choii (ici "Commune" "Population" et
-- "Pourcentage") dans l'affichage des r�sultats

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

