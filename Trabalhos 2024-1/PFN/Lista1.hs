--Qquestão 1
ehTriangulo x y z = 
    if x+y > z && x+z > y && z+y > x then True
    else False

--Questão 2
tipoTriangulo x y z =
    if x == y && x == z then "equilatero"
    else if x == y && x/=z || x == z && x/= y || z == y && z/= x then "isosceles"
    else "escaleno"

-- Questão 3
triangulo x y z =
    if x+y <= z || x+z <= y || z+y <= x then "nao eh um triangulo"
    else if  x == y && x == z then "equilatero"
    else if x == y && x/=z || x == z && x/= y || z == y && z/= x then "isosceles"
    else "escaleno"

-- Questão 4
soma 0 = 0
soma 1 = 0
soma n = soma (n-2) + n 

somaPares n = 
    if rem n 2 == 0 then soma n
    else soma (n-1)

--Questão 5
somaPot2m m n 
              | n < 0 = 0
              | otherwise = 2^n * m + somaPot2m m (n-1)

-- Questão 6
prim f g | g <= 1 = True
       | mod f g == 0 = False
       | otherwise = prim f (g-1)

primo 1 = False
primo f = prim f (f-1)

-- Questão 7
seriePI n = pg 1 1 n 
pg 0 0 0 = 0
pg x y z 
         | z>= x = y*(4/x) + pg (x+2) (y*(-1)) z
         | otherwise = pg 0 0 0



     
    
    



    
    

