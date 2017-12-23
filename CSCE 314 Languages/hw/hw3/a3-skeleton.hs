
-- Assignment 3, CSCE-314
-- Section: PUT YOUR SECTION HERE
-- Student Name: PUT YOUR NAME HERE
-- UIN: PUT YOUR UIN HERE
-- (Acknowledge any help received here)

module Main where

import Test.HUnit
import System.Exit
import Data.List
import Data.Char

triads :: Int -> [(Int, Int, Int)]
triads = undefined

perfect :: [Int]
perfect = undefined

mergeSortBy :: (a -> a -> Bool) -> [a] -> [a]
mergeSortBy = undefined

mergeSort :: Ord a => [a] -> [a]
mergeSort = undefined

multiply :: [Int] -> Int
multiply = undefined

concatenate :: [String] -> String     
concatenate = undefined

concatenateAndUpcaseOddLengthStrings :: [String] -> String
concatenateAndUpcaseOddLengthStrings = undefined

insertionSort :: Ord a => [a] -> [a]
insertionSort = undefined

maxElementOfAList :: Ord a => [a] -> a 
maxElementOfAList = undefined

keepInBetween :: Int -> Int -> [Int] -> [Int]
keepInBetween = undefined

data Tree a b = Branch b (Tree a b) (Tree a b)
              | Leaf a

preorder  :: (a -> c) -> (b -> c) -> Tree a b -> [c]
postorder :: (a -> c) -> (b -> c) -> Tree a b -> [c]
inorder   :: (a -> c) -> (b -> c) -> Tree a b -> [c]
preorder = undefined
postorder = undefined
inorder = undefined

data E = IntLit Int
       | BoolLit Bool
       | Plus E E
       | Minus E E
       | Multiplies E E
       | Divides E E
       | Equals E E
         deriving (Eq, Show)

eval :: E -> E
eval = undefined


mytree = Branch "A" 
           (Branch "B" 
              (Leaf 1) 
              (Leaf 2)) 
           (Leaf 3)

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
