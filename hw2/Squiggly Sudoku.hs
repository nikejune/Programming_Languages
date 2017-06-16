
module Sudoku where


valList :: [Int]
valList = [0, 0, 3, 0, 0, 0, 2, 0, 5,
           0, 0, 0, 9, 0, 5, 0, 0, 7,
           5, 8, 0, 3, 0, 9, 0, 1, 4,
           0, 0, 1, 0, 0, 4, 0, 0, 0,
           9, 0, 0, 5, 1, 2, 0, 0, 3,
           0, 0, 0, 7, 0, 0, 9, 0, 0,
           7, 5, 0, 2, 0, 1, 0, 6, 9,
           3, 0, 0, 6, 0, 7, 0, 0, 0,
           1, 0, 6, 0, 0, 0, 8, 0, 0]

blkList :: [Int]
blkList = [1, 1, 2, 2, 2, 3, 3, 3, 3,
           1, 1, 2, 1, 2, 2, 2, 3, 3,
           1, 1, 1, 1, 5, 2, 2, 3, 3,
           4, 4, 4, 4, 5, 5, 6, 6, 3,
           4, 4, 4, 5, 5, 5, 6, 6, 6,
           7, 4, 4, 5, 5, 6, 6, 6, 6,
           7, 7, 8, 8, 5, 9, 9, 9, 9,
           7, 7, 8, 8, 8, 9, 8, 9, 9,
           7, 7, 7, 7, 8, 8, 8, 9, 9]

itop :: Int -> (Int, Int)
itop i = (calcX i, calcY i)
  where calcX i   = i - 9 * (i `div` 9)
        calcY i   = i `div` 9

ptoi :: (Int, Int) -> Int
ptoi (x, y) = x + y * 9

columnAt :: Int -> [Int] -> [Int]
columnAt p s = helperColumnAt (itop p) s
  where helperColumnAt (x, _) s = map (\y -> s !! ptoi (x, y)) [0..8]

rowAt :: Int -> [Int] -> [Int]
rowAt p s = helperRowAt (itop p) s
  where helperRowAt (_, y) s = map (\x -> s !! ptoi (x, y)) [0..8]

-- Retrieves the 3 x 3 square of values from the board (s) at the index (p)
blockAt :: Int -> [Int] -> [Int] -> [Int]
blockAt p s1 s2 = [s1 !! x | x<-[0..80], s2 !! x == s2 !! p]


-- Removes the elements in the second list from the first list
remove' :: [Int] -> [Int] -> [Int]
remove' [] _       = []
remove' xs []      = xs
remove' xs (y:ys)  = remove' (removeAll y xs) ys

-- Remove all occurences of a value in a list
removeAll :: Int -> [Int] -> [Int]
removeAll _ []     = []
removeAll y (x:xs) | x == y    = removeAll y xs
                   | otherwise = x : removeAll y xs

-- The list of solutions at the index p of board s
solutionsAt :: Int -> [Int] -> [Int] -> [Int]
solutionsAt p s1 s2 | p > length s1  = []
                    | (s1 !! p) == 0 = [1..9] `remove'` (columnAt p s1 ++ rowAt p s1 ++ blockAt p s1 s2)
                    | otherwise     = [s1 !! p]

-- Generate a new version of board s with value x inserted at index p
tryWith :: Int -> [Int] -> Int -> [Int]
tryWith p s x = take p s ++ [x] ++ drop (p + 1) s

-- Find the next blank value starting from index p on board s
-- 80 is the index of the last element in s
nextBlank :: Int -> [Int] -> Int
nextBlank p s | p == 80           = 80
              | s !! (p + 1) == 0 = p + 1
              | otherwise         = nextBlank (p + 1) s

-- Recursively try and brute-force solve the board given in s, starting at p,
-- with the set of possible solutions at that point.
-- 80 is the index of the last element in s
solve :: Int -> [Int] -> [Int] -> [Int] -> [Int]
solve 80 s1 s2 []     = []
solve 80 s1 s2 (x:[]) = tryWith 80 s1 x
solve 80 s1 s2 (x:_)  = []
solve _  s1 s2 []     = []
solve p s1 s2 (x:xs)  | solvedNext == [] = solve p s1 s2 xs
                  | otherwise        = solvedNext
  where solveNext p s1 s2 = solve (nextBlank p s1) s1 s2 (solutionsAt (nextBlank p s1) s1 s2)
        solvedNext    = solveNext p (tryWith p s1 x) s2

solveIt s1 s2 = solve 0 s1 s2 (solutionsAt 0 s1 s2)

-- intersperse the element c through-out the string xs
joinWith :: a -> [a] -> [a]
joinWith _ (x:[])  = [x]
joinWith c (x:xs)  = x : c : joinWith c xs

-- Pretty-print the board as a spaced out 9 x 9 square
pPrint [] = []
pPrint s  = spaceOut s ++ pPrint (drop 9 s)
  where showS s    = concatMap show s
        space      = ' '
        spaceOut s = joinWith space (take 9 (showS s) ++"\n")

squigglySudoku :: [Int] -> [Int] -> [Char]
squigglySudoku s1 s2 = pPrint (solveIt s1 s2)
