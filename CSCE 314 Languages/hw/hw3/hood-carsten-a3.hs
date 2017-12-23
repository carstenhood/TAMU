
-- Assignment 3, CSCE-314
-- Section: 502
-- Student Name: CARSTEN HOOD
-- UIN: 922009787
-- Elliott Dobbs

module Main where

import Test.HUnit
import System.Exit
import Data.List
import Data.Char

triads :: Int -> [(Int, Int, Int)]
triads n = [(x, y, z) | x <- [1..n],
                        y <- [1..n],
                        z <- [1..n],
                        x^2 + y^2 == z^2,
                        x <= y]

perfect :: [Int]
perfect =   let factors n = [f | f <- [1..(n-1)], n `mod` f == 0]
            in [x | x <- [1..], sum (factors x) == x]

mergeSortBy :: (a -> a -> Bool) -> [a] -> [a]
mergeSortBy comp [] = []
mergeSortBy comp [x] = [x]
mergeSortBy comp xs = mergeBy comp (mergeSortBy comp xs1) (mergeSortBy comp xs2)
    where (xs1, xs2) = split xs

split :: [a] -> ([a],[a])
split xs = fork xs [] [] where
   fork [] as bs = (as, bs)
   fork [a] as bs = (a:as, bs)
   fork (a:b:xs) as bs = fork xs (a:as) (b:bs)

mergeBy :: (a -> a -> Bool) -> [a] -> [a] -> [a]
mergeBy comp xs [] = xs
mergeBy comp [] ys = ys
mergeBy comp (x:xs) (y:ys)
    | comp x y = x: mergeBy comp xs (y:ys)
    | otherwise = y: mergeBy comp (x:xs) ys

mergeSort :: Ord a => [a] -> [a]
mergeSort [] = []
mergeSort [x] = [x]
mergeSort xs = merge (mergeSort xs1) (mergeSort xs2)
    where (xs1, xs2) = split xs

merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys)
    | x <= y = x: merge xs (y:ys)
    | otherwise = y: merge (x:xs) ys

multiply :: [Int] -> Int
multiply = foldr (*) 1

concatenate :: [String] -> String     
concatenate = foldl (++) ""

concatenateAndUpcaseOddLengthStrings :: [String] -> String
concatenateAndUpcaseOddLengthStrings vs = concatenate (map (map toUpper) (filter (odd . length) vs))

insertionSort :: Ord a => [a] -> [a]
insertionSort [] = []
insertionSort (x:xs) = insert x (insertionSort xs)

maxElementOfAList :: Ord a => [a] -> a
maxElementOfAList = foldl1 (\x -> (\y -> if x >= y then x else y))

keepInBetween :: Int -> Int -> [Int] -> [Int]
keepInBetween a b xs = filter (>= a) (filter (<= b) xs)

data Tree a b = Branch b (Tree a b) (Tree a b)
              | Leaf a

instance (Show a, Show b) => Show (Tree a b) where
    show t = showIndent "" t where
        showIndent indent (Leaf x) = indent ++ show x ++ "\n"
        showIndent indent (Branch x l r) = indent ++ show x ++ "\n" ++ showIndent newIndent l ++ showIndent newIndent r where
            newIndent = indent ++ "  "

preorder  :: (a -> c) -> (b -> c) -> Tree a b -> [c]
preorder fa fb (Leaf x) =  [fa x]
preorder fa fb (Branch x l r) = [fb x] ++ (preorder fa fb l) ++ (preorder fa fb r)

postorder :: (a -> c) -> (b -> c) -> Tree a b -> [c]
postorder fa fb (Leaf x) =  [fa x]
postorder fa fb (Branch x l r) = (postorder fa fb l) ++ (postorder fa fb r) ++ [fb x]

inorder   :: (a -> c) -> (b -> c) -> Tree a b -> [c]
inorder fa fb (Leaf x) =  [fa x]
inorder fa fb (Branch x l r) = (inorder fa fb l) ++ [fb x] ++ (inorder fa fb r)


data E = IntLit Int
       | BoolLit Bool
       | Plus E E
       | Minus E E
       | Multiplies E E
       | Divides E E
       | Equals E E
         deriving (Eq, Show)

eval :: E -> E
eval (IntLit x) = IntLit x
eval (BoolLit x) = BoolLit x
eval (Plus x y) = plus (eval x) (eval y) where
    plus (IntLit a) (IntLit b) = IntLit (a + b)
    plus a b = error ("Cannot process addition between " ++ show a ++ " and " ++ show b ++ "\n")
eval (Minus x y) = minus (eval x) (eval y) where
    minus (IntLit a) (IntLit b) = IntLit (a - b)
    minus a b = error ("Cannot process subtraction between " ++ show a ++ " and " ++ show b ++ "\n")
eval (Multiplies x y) = multiplies (eval x) (eval y) where
    multiplies (IntLit a) (IntLit b) = IntLit (a * b)
    multiplies a b = error ("Cannot process multiplication between " ++ show a ++ " and " ++ show b ++ "\n")
eval (Divides x y) = divides (eval x) (eval y)  where
    divides (IntLit a) (IntLit b) = IntLit (a `div` b)
    divides a b = error ("Cannot process division between " ++ show a ++ " and " ++ show b ++ "\n")
eval (Equals x y) = equals (eval x) (eval y) where
    equals (BoolLit a) (BoolLit b) = BoolLit (a == b)
    equals (IntLit a) (IntLit b) = BoolLit (a == b)
    equals a b = error ("Cannot process equality between " ++ show a ++ " and " ++ show b ++ "\n")

mytree = Branch "A"
           (Branch "B"
              (Leaf 1)
              (Leaf 2))
           (Leaf 3)

mytree2 = Branch "A" (Branch "B" (Leaf 1) (Leaf 2)) (Branch "C" (Branch "D" (Leaf 4) (Leaf 5)) (Leaf 3))

program = Equals
            (Plus (IntLit 1) (IntLit 2))
            (Minus
             (IntLit 5)
             (Minus (IntLit 3) (IntLit 1)))
    
myTestList =

  let te s e a = test $ assertEqual s e a
      tb s b = test $ assertBool s b

  in
    TestList [ 
        tb "triads 1" $ [(3, 4, 5)] == triads 5
      , tb "triads 2" $ [(3, 4, 5), (6, 8, 10)] == ( sort $ triads 10)

      , tb "perfect" $ take 3 perfect == [6, 28, 496]
        
      , te "mergeSort 1" " adhllowy" (mergeSort "howdy all") 
      , te "mergeSort 2" "" (mergeSort "") 
      , te "mergeSort 3" "x" (mergeSort "x") 
        
      , te "mergeSortBy 1" " 'eggim" (mergeSortBy (<) "gig 'em") 
      , te "mergeSortBy 2" "nmlkieecbbaJ  " (mergeSortBy (>) "Jack be nimble")
      , te "mergeSortBy 3" "" (mergeSortBy (<) "")

      , te "multiply" 10 (multiply [-2, -1, 5])
      , te "concatenate" "ABCD" (concatenate ["AB", "", "", "C", "D", ""])

      , te "concatenateAndUpcaseOddLengthStrings"
          "HERE'S AN EXAMPLE" (concatenateAndUpcaseOddLengthStrings ["here's ", "an ", "a ", "example"])

      , te "insertionSort" "  Jabcceikkqu" (insertionSort "Jack be quick")
      , te "max in a list" 100 (maxElementOfAList [3, 100, 0])
      , te "keepInBetween" [3,0,20,2] (keepInBetween 0 20 [77, 3, -1, 0, 21, 20, -9, 2])

      , te "preorder" "AB123" (concatenate (preorder show id mytree))
      , te "postrder" "12B3A" (concatenate (postorder show id mytree))
      , te "inorder" "1B2A3" (concatenate (inorder show id mytree))

      , te "eval" (BoolLit True) (eval program)
      ]

main = do c <- runTestTT myTestList
          putStrLn $ show c
          let errs = errors c
              fails = failures c
          exitWith (codeGet errs fails)
          
codeGet errs fails
 | fails > 0       = ExitFailure 2
 | errs > 0        = ExitFailure 1
 | otherwise       = ExitSuccess
