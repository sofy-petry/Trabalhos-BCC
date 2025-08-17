
import System.IO
import Data.List
import Data.Char

transforma _[] = []
transforma f (x:xs) = f x : transforma f xs
all2min =toLower

reduzir f [x] = x
reduzir f (x:xs) = f x (reduzir f xs)

redu f aux [] = aux
redu f aux (x:xs) = f x (redu f aux xs)

data Arvore a = Node a (Arvore a) (Arvore a) | Folha deriving Show
maiorArv (Node valor _ Folha) = valor
maiorArv (Node _ _ dir) = maiorArv dir

menorArv (Node valor Folha _ ) = valor 
menorArv (Node _ esq _ ) = menorArv esq

arv = Node 6 (Node 3 Folha Folha) (Node 10 Folha (Node 15 Folha Folha))

-- acessar todos os nos
todosnos Folha =[]
todosnos (Node n Folha Folha) = [n]
todosnos (Node n esq dir) = n:todosnos esq ++ todosnos dir

-- acessar só folhas 
folhas Folha = []
folhas (Node n Folha Folha)= [n]
folhas (Node n esq dir) = folhas esq ++ folhas dir

--acessar só no
soNo Folha =[]
soNo (Node n Folha Folha) = []
soNo (Node n esq dir ) = n: soNo esq ++ soNo dir


--2
compDuplas [] = []
compDuplas ((x,y): xs) = ((y,x): compDuplas xs)

-- 3
zip' _ [] = []
zip' [] _ = []
zip' (x:xs) (y:ys) = ((x,y): zip' xs ys)

-- 4
aux [] = []
aux (x:xs)= if x== 'a' || x=='e' || x=='i' || x=='o' || x== 'u' then aux xs
                  else x:aux xs
semVogal [] = []
semVogal ((x:xs):y) = (aux (x:xs): semVogal y) 



