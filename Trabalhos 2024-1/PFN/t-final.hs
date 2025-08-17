import System.IO
import Data.List
import Data.Char

data Arvore = Node [Int] String Arvore Arvore | Folha deriving Show

imprimir Folha = return ()
imprimir (Node l p esq dir) = do imprimir esq
                                 putStrLn (p ++ " - " ++ show l)
                                 imprimir dir
minus = map toLower

removPonto [] = []
removPonto (x:xs) = if not(isDigit x) && not(isAlpha x) && not(isSpace x) then removPonto xs
                      else x:removPonto xs

--separar o texto em linhas e a numerá-las
numLines' n [] = []
numLines' n (x:xs) = (n, x): numLines' (n+1) xs
numLines xs = numLines' 1 (lines xs)

--separar cada palavra e dizer sua linha
allNumWords [] = []
allNumWords ((num, linha):xs) = zip (repeat num) (words linha) ++ allNumWords xs

--Inserir elementos
insOrd n [] = [n]
insOrd n y@(x:xs)  |n == x = y
                   |n < x = (n:y)
                   |otherwise = (x:insOrd n xs)

--inserindo na arvore
ins Folha y x = Node [y] x  Folha Folha
ins (Node i s esq dir) y x    |x == s = Node (insOrd y i) s esq dir
                              |x < s = Node i s (ins esq y x) dir
                              |otherwise = Node i s esq (ins dir y x)

--Fazer o index
mIndexTree [] arv = arv
mIndexTree ((y,x):xs) arv = mIndexTree xs (ins arv y x)

main = do putStr "Arquivo: "
          hFlush stdout
          arq <- getLine
          txt <- readFile arq
          let minusculos = minus txt
          let pontos = removPonto minusculos
          let linhas = numLines pontos
          let palavras = allNumWords linhas
          let index = mIndexTree palavras Folha
          imprimir index