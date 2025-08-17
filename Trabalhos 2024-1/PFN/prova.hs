--p1
restante n [] = []
restante n (x:xs)= if n == x then xs else restante n xs

enesimo n (x:xs) = if n == 0 then x else enesimo (n-1) xs

replicarElem 0 _ = []
replicarElem n x = x : replicarElem (n-1) x

replicar _ [] = []
replicar n (x:xs) = replicarElem n x ++ replicar n xs

--p2
compDuplas [] = []
compDuplas ((x,y):xs) = ((y,x): compDuplas xs)

zip' _ [] = []
zip' [] _ = []
zip' (x:xs) (y:ys) = ((x,y): zip xs ys)

removeV [] = []
removeV (x:xs) = if x == 'a' || x == 'e' || x == 'i' || x == 'o' || x =='u' then removeV xs else [x] ++ removeV xs
removeVogal [] = []
removeVogal ((x:xs):z) = (removeV (x:xs): removeVogal (z))