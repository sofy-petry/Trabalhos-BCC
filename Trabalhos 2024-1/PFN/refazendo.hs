import Data.Char

--função que verifica se um elemento pertence a uma lista
pertence n [] = False 
pertence n (x:xs) = if n == x then True else pertence n xs 

-- função que retorne a interseção entre duas listas
intersecao _ [] = []
intersecao [] _ = []
intersecao (x:xs) (y:ys)= if pertence x (y:ys) then x: intersecao xs  (y:ys) else intersecao xs (y:ys)

--função que retorne o inverso de uma lista
inverso [] = []
inverso (x:xs) = inverso xs ++ [x]

--função que retorne os n últimos elementos de uma lista
nPrimeiros 0 _ = []
nPrimeiros _ [] = []
nPrimeiros n (x:xs)= x: nPrimeiros (n-1) xs

nUltimos n (x:xs) = inverso(nPrimeiros n ( inverso (x:xs)))

{--função que receba duas listas de números e crie uma lista com a soma do 
primeiro elemento da primeira lista com o primeiro elemento da segunda lista--}
soma2 _ [] = []
soma2 [] _ = []
soma2 (x:xs) (y:ys) = x + y : soma2 xs ys

--função que receba como parâmetro um número n e retorne uma lista com todas as potências de 2 até 2n
pot 0 = []
pot n = (2^n) : pot (n-1)
pot2 n = inverso (pot n)

{--função que receba duas listas previamente ordenadas e faça a intercalação
(merge) dos elementos tendo como resultado a junção das duas listas em uma lista também
ordenada, sem reordenar nenhuma das listas--}
intercalacao xs [] = xs
intercalacao [] ys = ys
intercalacao (x:xs) (y:ys) = if x> y then y:intercalacao (x:xs) (ys) else if y > x then x :intercalacao (xs) (y:ys)
else x:y:intercalacao xs ys

--função que retorne o menor elemento de uma lista
menor [x] = x
menor (x:xs) = if x < menor (xs) then x else menor xs

--receba uma lista e um elemento e retorne a lista sem a primeira ocorrência desse elemento
removerElem n [] =[]
removerElem n (x:xs) = if n == x then xs else [x] ++ removerElem n xs

-- função que ordene os elementos de uma lista
ordenar [] = []
ordenar (xs) = menor (xs) : ordenar (removerElem (menor xs) xs)

{--função que receba um elemento e uma lista ordenada, e insira este elemento na
lista o colocando na posição correta, ou seja, a lista resultante deve estar ordenada--}
insereOrd n [] = [n]
insereOrd n (x:xs) = if n < x then n:(x:xs) else if n `elem` (x:xs) then (x:xs) else [x]++ insereOrd n xs

--função que receba um número n e uma lista, e retorne o n-ésimo elemento
enesimo n [] = 0
enesimo 0 _ = 0
enesimo n (x:xs) = if n == 1 then x else enesimo (n-1) xs

--função que receba um inteiro n e um elemento e e crie uma lista com n elementos e
repetir 0 _ = []
repetir n e = e: repetir (n-1) e 

--função que troque todos os caracteres de tabulação (‘\t’) por espaços em uma String
removeTab [] = []
removeTab (x:xs) = if x == '\t' then ' ': removeTab (xs) else [x] ++ removeTab (xs)

--função que receba uma String e converta todas as letras maiúsculas dessa String em letras minúsculas
minusculas [] = []
minusculas (x:xs) = toLower x : minusculas xs

--Declare uma função que receba uma lista de duplas e retorne uma lista com o inverso de cada dupla
inversoDupla [] = []
inversoDupla ((x,y):z) = ((y,x): inversoDupla z)

{--função que receba uma lista de duplas, e retorne lista de booleanos indicando se 
os elementos são iguais ou não--}
simetrico [] = []
simetrico ((x,y):z) = if x == y then True: simetrico z else False: simetrico z
