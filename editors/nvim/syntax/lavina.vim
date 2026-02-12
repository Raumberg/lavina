if exists("b:current_syntax")
  finish
endif

" Comments
syn match lavinaComment "//.*$"

" Strings with interpolation
syn region lavinaString start='"' end='"' contains=lavinaEscape,lavinaInterp
syn match lavinaEscape "\\." contained
syn region lavinaInterp start='\${' end='}' contained contains=TOP

" Numbers
syn match lavinaFloat "\<\d\+\.\d\+\>"
syn match lavinaInt "\<\d\+\>"

" Control flow
syn keyword lavinaControl if elif else while for in return break continue match try catch throw

" Import
syn keyword lavinaImport import as

" Logical operators
syn keyword lavinaLogical and or not

" Definitions
syn keyword lavinaKeyword fn constructor public private static inline const let comptime namespace

" Storage
syn keyword lavinaStorage class struct enum

" Types
syn keyword lavinaType int float string bool void auto dynamic vector hashmap hashset null

" Constants
syn keyword lavinaConstant true false null
syn keyword lavinaThis this

" Functions (name after fn)
syn match lavinaFuncDef "\<fn\s\+\zs\w\+"
" Class/enum name
syn match lavinaTypeDef "\<\(class\|struct\|enum\)\s\+\zs\w\+"

" Operators
syn match lavinaOperator "&&\|||\|!\|==\|!=\|<=\|>=\|<\|>\|+=\|-=\|\*=\|::\|->\|\.\."

" Highlighting
hi def link lavinaComment Comment
hi def link lavinaString String
hi def link lavinaEscape SpecialChar
hi def link lavinaInterp Special
hi def link lavinaFloat Float
hi def link lavinaInt Number
hi def link lavinaControl Conditional
hi def link lavinaImport Include
hi def link lavinaLogical Keyword
hi def link lavinaKeyword Keyword
hi def link lavinaStorage Structure
hi def link lavinaType Type
hi def link lavinaConstant Boolean
hi def link lavinaThis Constant
hi def link lavinaFuncDef Function
hi def link lavinaTypeDef Type
hi def link lavinaOperator Operator

let b:current_syntax = "lavina"
