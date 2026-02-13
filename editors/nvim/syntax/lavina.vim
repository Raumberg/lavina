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
syn keyword lavinaControl if elif else while for in return break continue match try catch throw pass

" Import
syn keyword lavinaImport import as

" Logical operators
syn keyword lavinaLogical and or not

" Definitions
syn keyword lavinaKeyword fn constructor public private static inline const let comptime namespace ref own cpp extern link type operator

" Storage
syn keyword lavinaStorage class struct enum

" Types
syn keyword lavinaType int float string bool void auto dynamic vector hashmap hashset null int8 int16 int32 int64 float32 float64 usize cstring ptr

" Constants
syn keyword lavinaConstant true false null
syn keyword lavinaThis this

" Functions (name after fn)
syn match lavinaFuncDef "\<fn\s\+\zs\w\+"
" Class/struct/enum name in definitions
syn match lavinaTypeDef "\<\(class\|struct\|enum\)\s\+\zs\w\+"
" User types: PascalCase identifiers (starts with uppercase)
syn match lavinaUserType "\<[A-Z][a-zA-Z0-9_]*\>"

" Function calls: identifier followed by (
syn match lavinaFuncCall "\<[a-z_][a-zA-Z0-9_]*\ze\s*("

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
hi def link lavinaFuncCall Function
hi def link lavinaTypeDef Type
hi def link lavinaUserType Type
hi def link lavinaOperator Operator

let b:current_syntax = "lavina"
