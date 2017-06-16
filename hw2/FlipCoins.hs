flipCoin_ :: [Char] -> ( Int -> [Int] )

flipCoin_ [] y = [0]

flipCoin_ [x] y
	| x == 'H' = [0]
	| otherwise = [y+1, 0]

flipCoin_ (x:xs) y
	| x == (head xs) = flipCoin_ xs (y+1)
	| otherwise = [y+1] ++ flipCoin_ xs (y+1)

flipCoin :: [Char] -> [Int]
flipCoin xs = flipCoin_ xs 0
