
-- Assignment p4, CSCE-314
-- Section: 502
-- Student Name: CARSTEN HOOD
-- UIN: 922009787
-- ( N/A )

module Main where

import Prelude hiding (lookup)

import Test.HUnit
import System.Exit

-- AST definition for W
data WValue = VInt Int 
            | VBool Bool 
              deriving (Eq, Show)

data WExp = Val WValue
          | Var String

          | Plus WExp WExp
          | Minus WExp WExp
          | Multiplies WExp WExp
          | Divides WExp WExp

          | Equals WExp WExp
          | NotEqual WExp WExp
          | Less WExp WExp
          | Greater WExp WExp
          | LessOrEqual WExp WExp
          | GreaterOrEqual WExp WExp

          | And WExp WExp
          | Or WExp WExp
          | Not WExp

data WStmt = Empty
           | VarDecl String WExp
           | Assign String WExp
           | If WExp WStmt WStmt
           | While WExp WStmt
           | Block [WStmt]

type Memory = [(String, WValue)]
marker = ("|", undefined)
isMarker (x, _) = x == "|"

--- eval helper functions
fPlus :: WValue -> WValue -> WValue
fPlus (VInt a) (VInt b) = VInt (a + b)
fPlus a b = error ("cannot add " ++ show a ++ " and " ++ show b)

fMinus :: WValue -> WValue -> WValue
fMinus (VInt a) (VInt b) = VInt (a - b)
fMinus a b = error ("cannot subtract " ++ show a ++ " and " ++ show b)

fMultiplies :: WValue -> WValue -> WValue
fMultiplies (VInt a) (VInt b) = VInt (a * b)
fMultiplies a b = error ("cannot multiply " ++ show a ++ " and " ++ show b)

fDivides :: WValue -> WValue -> WValue
fDivides (VInt a) (VInt b) = VInt (a `div` b)
fDivides a b = error ("cannot divide " ++ show a ++ " and " ++ show b)

fEquals :: WValue -> WValue -> WValue
fEquals (VInt a) (VInt b) = VBool (a == b)
fEquals (VBool a) (VBool b) = VBool (a == b)
fEquals a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fNotEqual :: WValue -> WValue -> WValue
fNotEqual (VInt a) (VInt b) = VBool (a /= b)
fNotEqual (VBool a) (VBool b) = VBool (a /= b)
fNotEqual a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fLess :: WValue -> WValue -> WValue
fLess (VInt a) (VInt b) = VBool (a < b)
fLess a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fGreater :: WValue -> WValue -> WValue
fGreater (VInt a) (VInt b) = VBool (a > b)
fGreater a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fLessOrEqual :: WValue -> WValue -> WValue
fLessOrEqual (VInt a) (VInt b) = VBool (a <= b)
fLessOrEqual a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fGreaterOrEqual :: WValue -> WValue -> WValue
fGreaterOrEqual (VInt a) (VInt b) = VBool (a >= b)
fGreaterOrEqual a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fAnd :: WValue -> WValue -> WValue
fAnd (VBool a) (VBool b) = VBool (a && b)
fAnd a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fOr :: WValue -> WValue -> WValue
fOr (VBool a) (VBool b) = VBool (a || b)
fOr a b = error ("cannot compare " ++ show a ++ " and " ++ show b)

fNot :: WValue -> WValue
fNot (VBool a) = VBool (not a)
fNot a = error ("cannot negate " ++ show a)

-- eval function
eval :: WExp -> Memory -> WValue
eval (Val x) m = x
eval (Var x) m = fromJust (lookup x m)
eval (Plus x y) m = fPlus (eval x m) (eval y m)
eval (Minus x y) m = fMinus (eval x m) (eval y m)
eval (Multiplies x y) m = fMultiplies (eval x m) (eval y m)
eval (Divides x y) m = fDivides (eval x m) (eval y m)
eval (Equals x y) m = fEquals (eval x m) (eval y m)
eval (NotEqual x y) m = fNotEqual (eval x m) (eval y m)
eval (Less x y) m = fLess (eval x m) (eval y m)
eval (Greater x y) m = fGreater (eval x m) (eval y m)
eval (LessOrEqual x y) m = fLessOrEqual (eval x m) (eval y m)
eval (GreaterOrEqual x y) m = fGreaterOrEqual (eval x m) (eval y m)
eval (And x y) m = fAnd (eval x m) (eval y m)
eval (Or x y) m = fOr (eval x m) (eval y m)
eval (Not x) m = fNot (eval x m)

-- exec function
exec :: WStmt -> Memory -> Memory
exec (Empty) m = m
exec (VarDecl s e) m
    | (lookupScope s m) == Nothing = (s, (eval e m)):m
    | otherwise = error ("variable '" ++ s ++ "' already declared")
exec (Assign s e) m = repFDefF s (eval e m) m where
    repFDefF _ _ [] = error ("variable '" ++ s ++ "' undefined")
    repFDefF s v (x@(k, _):xs)
        | s == k =
            if (sameKind v x) then (k, v):xs
            else error ("type mismatch in assignment of variable '" ++ s ++ "'")
        | otherwise = x:(repFDefF s v xs)
        where
            sameKind v@(VInt _) x@(_, VInt _) = True
            sameKind v@(VBool _) x@(_, VBool _) = True
            sameKind _ _ = False
exec (Block ss) m = popMarker $ foldr exec (marker:m) (reverse ss)
    where
        popMarker [] = []
        popMarker (x:xs) | isMarker x = xs | otherwise = popMarker xs
exec (If e s1 s2) m
    | asBool (eval e m) = exec s1 m
    | otherwise = exec s2 m
exec (While e s) m
    | asBool (eval e m) = exec (While e s) (exec s m)
    | otherwise = m

-- fibonacci
fibonacci :: Int -> Int
fibonacci n = asInt (fromJust (lookup "result" (
    exec fibonacciProgram [("result", VInt (-1)), ("n", VInt n)])))

fibonacciProgram =
    Block [
        VarDecl "fib" (Val (VInt 0)),
        If (Equals (Var "n") (Val (VInt(1))))
            ( Block [Assign "fib" (Val (VInt 1))])
            ( Block [
                VarDecl "v1" (Val (VInt 0)),
                VarDecl "v2" (Val (VInt 1)),
                VarDecl "i" (Val (VInt 1)),
                While (Less (Var "i") (Var "n")) (
                    Block [
                        Assign "fib" (Plus (Var "v1") (Var "v2")),
                        Assign "v1" (Var "v2"),
                        Assign "v2" (Var "fib"),
                        Assign "i" (Plus (Var "i") (Val (VInt 1)))
                    ]
                )
            ]
        ),
        Assign "result" (Var "fib")
    ]

-- example programs
prog1 = Block
     [
       VarDecl "x" (Val (VInt 0)),
       VarDecl "b" (Greater (Var "x") (Val (VInt 0))),
       If (Or (Var "b") (Not (GreaterOrEqual (Var "x") (Val (VInt 0)))))
         ( Block [ Assign "x" (Val (VInt 1)) ] )
         ( Block [ Assign "x" (Val (VInt 2)) ] )
     ]

factorial = Block
  [
    VarDecl "acc" (Val (VInt 1)),
    While (Greater (Var "arg") (Val (VInt 1)))
    (
      Block
      [
        Assign "acc" (Multiplies (Var "acc") (Var "arg")),
        Assign "arg" (Minus (Var "arg") (Val (VInt 1)))         
      ]
    ),
    Assign "result" (Var "acc")
  ]

-- programs to test basic operations
test1 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Plus (Var "x") (Var "y"))
    ]
test2 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Minus (Var "x") (Var "y"))
    ]
test3 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Multiplies (Var "x") (Var "y"))
    ]
test4 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Divides (Var "x") (Var "y"))
    ]
test5 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Equals (Var "x") (Var "y"))
    ]
test6 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (NotEqual (Var "x") (Var "y"))
    ]
test7 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Less (Var "x") (Var "y"))
    ]
test8 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (Greater (Var "x") (Var "y"))
    ]
test9 = Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (LessOrEqual (Var "x") (Var "y"))
    ]
test10 =
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (GreaterOrEqual (Var "x") (Var "y"))
    ]
test11 =
    Block [
        VarDecl "x" (Val (VBool True)),
        VarDecl "y" (Val (VBool False)),
        Assign "result" (And (Var "x") (Var "y"))
    ]
test12 =
    Block [
        VarDecl "x" (Val (VBool True)),
        VarDecl "y" (Val (VBool False)),
        Assign "result" (Or (Var "x") (Var "y"))
    ]
test13 =
    Block [
        VarDecl "x" (Val (VBool False)),
        Assign "result" (Not (Var "x"))
    ]

-- double-loop program for testing statements, scope and complex operations
test14 =
    Block [
        VarDecl "counter" (Val (VInt (0))),
        VarDecl "i" (Val (VInt 0)),
        VarDecl "n" (Val (VInt 10)),
        While (Not (GreaterOrEqual (Var "i") (Var "n"))) ( Block [
            Assign "i" (Plus (Var "i") (Val (VInt 1))),
            VarDecl "i" (Val (VInt 0)), -- test var with same name in new scope
            VarDecl "n" (Val (VInt 10)),
            While (Less (Var "i") (Var "n")) ( Block [
                Assign "i" (Plus (Var "i") (Val (VInt 1))),
                If (Equals (Var "i") (Var "n"))
                    ( Block [
                        Assign "counter" (Plus (Var "counter") (Var "i"))
                    ] )
                    ( Block [
                        -- test empty block
                    ] )
            ] )
        ] ),
        Assign "result" (Var "counter")
    ]

-- program for testing logical operators
test15 =
    Block [
        VarDecl "x" (Val (VBool True)),
        VarDecl "y" (Val (VBool False)),
        Assign "result" (Or (And (Var "x") (Var "y")) (Not (Var "y")))
    ]

-- programs for testing errors
test16 =    -- repeated declaration in same scope
    Block [
        VarDecl "x" (Val (VBool True)),
        VarDecl "x" (Val (VBool False))
    ]
test17 =    -- no declaration
    Block [
        Assign "x" (Val (VBool True)),
        Assign "result" (Var "x")
    ]
test18 =    -- Int operation on Bool
    Block [
        VarDecl "x" (Val (VBool True)),
        VarDecl "y" (Val (VBool False)),
        Assign "result" (Multiplies (Var "x") (Var "y"))
    ]
test19 =    -- Bool operation on Int
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VInt 3)),
        Assign "result" (And (Var "x") (Var "y"))
    ]
test20 =    -- wrong assignment type
    Block [
        VarDecl "x" (Val (VInt 7)),
        VarDecl "y" (Val (VBool False)),
        Assign "y" (Var "x"),
        Assign "result" (Var "y")
    ]


-- helper functions
lookup s [] = Nothing
lookup s ((k,v):xs) | s == k = Just v
                    | otherwise = lookup s xs

lookupScope s [] = Nothing
lookupScope s (x@(k,v):xs)
    | isMarker x = Nothing
    | s == k = Just v
    | otherwise = lookupScope s xs

asInt (VInt v) = v
asInt x = error $ "Expected a number, got " ++ show x

asBool (VBool v) = v
asBool x = error $ "Expected a boolean, got " ++ show x

fromJust (Just v) = v
fromJust Nothing = error ("expected a value in Maybe, but got Nothing")

getIntResult :: WStmt -> Int
getIntResult test = asInt (fromJust (lookup "result" (exec test [("result", VInt (-1))])))

getBoolResult :: WStmt -> Bool
getBoolResult test = asBool (fromJust (lookup "result" (exec test [("result", VBool undefined)])))

-- unit tests
myTestList =
  TestList [

    -- test default programs
    test $ assertEqual "prog1 test" [] (exec prog1 []),
    let res = lookup "result" (exec factorial [("result", VInt (-1)), ("arg", VInt 10)])
    in test $ assertBool "factorial of 10" (3628800 == asInt (fromJust res)),

    -- test basic operations
    "addition" ~: getIntResult test1 ~=? 10,
    "subtraction" ~: getIntResult test2 ~=? 4,
    "multiplication" ~: getIntResult test3 ~=? 21,
    "division" ~: getIntResult test4 ~=? 2,
    "equal" ~: getBoolResult test5 ~=? False,
    "not equal" ~: getBoolResult test6 ~=? True,
    "less" ~: getBoolResult test7 ~=? False,
    "greater" ~: getBoolResult test8 ~=? True,
    "less or equal" ~: getBoolResult test9 ~=? False,
    "greater or equal" ~: getBoolResult test10 ~=? True,
    "and" ~: getBoolResult test11 ~=? False,
    "or" ~: getBoolResult test12 ~=? True,
    "not" ~: getBoolResult test13 ~=? True,

    -- test statements and complex operations
    "double loop" ~: getIntResult test14 ~=? 100,
    "logic evaluation" ~: getBoolResult test15 ~=? True,

    -- test fibonacci
    "fibonacci of 0" ~: fibonacci 0 ~=? 0,
    "fibonacci of 1" ~: fibonacci 1 ~=? 1,
    "fibonacci of 2" ~: fibonacci 2 ~=? 1,
    "fibonacci of 3" ~: fibonacci 3 ~=? 2,
    "fibonacci of 10" ~: fibonacci 10 ~=? 55,

    -- test errors
    -- ***INTENTIONAL ERRORS***
    "testing error intentionally: re-declaring variable in same scope" ~: getBoolResult test16 ~=? False,
    "testing error intentionally: assigning undefined variable" ~: getBoolResult test17 ~=? False,
    "testing error intentionally: int operation on boolean" ~: getIntResult test18 ~=? -1,
    "testing error intentionally: boolean operation on int" ~: getBoolResult test19 ~=? True,
    "testing error intentionally: looking up nonexistant variable" ~: asInt (fromJust (lookup "nonexistantVar" (exec test1 [("result", VInt (-1))]))) ~=? -1,
    "testing error intentionally: type mismatch in variable assignment" ~: getIntResult test20 ~=? -1
    ]

-- main: run the unit tests
main = do c <- runTestTT myTestList
          putStrLn $ show c
          let errs = errors c
              fails = failures c
          if (errs + fails /= 0) then exitFailure else return ()

