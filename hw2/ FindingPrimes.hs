canDividing :: Int -> Int
canDividing = floor.sqrt.fromIntegral


isPrime :: Int -> Bool
isPrime 1 = False
isPrime x = ([] == [y| y <- [2..canDividing(x)], mod x y == 0])


findingPrimes :: Int -> Int -> [Int]
findingPrimes a b = take b (filter isPrime[a..])
