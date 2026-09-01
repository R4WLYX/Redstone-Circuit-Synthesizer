```
<spec> ::= "spec" <identifier> "{" <declaration-sequence> "}"

<declaration-sequence> ::= <declaration> <newline>
						 | <declaration> <newline> <declaration-sequence>

<declaration> ::= <input-declaration>
                | <state-declaration>
                | <output-declaration>
                | <label-declaration>
                | <behavior-declaration>

<input-declaration> ::= "input" <identifier> ":" <type>

<state-declaration> ::= "state" <identifier> ":" <type>

<output-declaration> ::= "output" <identifier> ":" <type>
					   | "output" <identifier>

<type> ::= "B"
		 | "S"

<label-declaration> ::= "label" <identifier>

<behavior-declaration> ::= "behavior" <block>

<expression> ::= <if-expression>
			   | <switch-expression>
			   | <signal-expression>

<if-expression> ::= "if" <comparison-expression>
				    "then" <signal-expression>
					"else" <signal-expression>

<switch-expression> ::= "switch" <identifier> "{" <case-sequence> "}"

<case-sequence> ::= <case>
			      | <case> <case-sequence>

<case> ::= "case" <number> ":" <case-expression>
		 | "case" <identifier> ":" <case-expression>
		 | "default" ":" <case-expression>

<case-expression> ::= <signal-expression>

<comparison-expression> ::= <signal-expression>
						  | <signal-expression> "==" <signal-expression>
						  | <signal-expression> "!=" <signal-expression>

<signal-expression> ::= <conditional-expression>

<conditional-expression> ::= <or-expression>
						   | <or-expression> "?" <or-expression>

<or-expression> ::= <sub-expression>
				  | <or-expression> "|" <sub-expression>

<sub-expression> ::= <unary-expression>
				   | <sub-expression> "-" <unary-expression>

<unary-expression> ::= <primary>
					 | "~" <unary-expression>

<primary> ::= <identifier>
            | <identifier> "[" <number> "]"
            | <identifier> "(" <number> ")"
            | <number>
            | "(" <signal-expression> ")"
            
<block> ::= "{" <statement-sequence> "}"

<statement-sequence> ::= <statement> <newline>
                       | <statement> <newline> <statement-sequence>

<statement> ::= <for-statement>
              | <assignment>

<assignment> ::= <identifier> "=" <expression>
               | "next" <identifier> "=" <expression>

<for-statement> ::= "forall" <identifier> "in" <range> <block>

<range> ::= "0" "..." <number>

<identifier> ::= <letter> <identifier-tail>
			   | "_" <identifier-tail>

<identifier-tail> ::= ε
                    | <character> <identifier-tail>

<character> ::= <letter>
			  | <digit>
			  | "_"

<letter> ::= "a" | "b" | "c" | "d" | "e" | "f" | "g"
		   | "h" | "i" | "j" | "k" | "l" | "m" | "n"
		   | "o" | "p" | "q" | "r" | "s" | "t" | "u"
		   | "v" | "w" | "x" | "y" | "z"
		   | "A" | "B" | "C" | "D" | "E" | "F" | "G"
		   | "H" | "I" | "J" | "K" | "L" | "M" | "N"
		   | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
		   | "V" | "W" | "X" | "Y" | "Z"

<number> ::= <decimal-number>
		   | <hex-number>

<decimal-number> ::= <digit>
				   | <digit> <decimal-number>

<hex-number> ::= "0x" <hex-digit-sequence>
			   | "0X" <hex-digit-sequence>

<hex-digit-sequence> ::= <hex-digit>
					   | <hex-digit> <hex-digit-sequence>

<hex-digit> ::= <digit>
			  | "a" | "b" | "c" | "d" | "e" | "f"
			  | "A" | "B" | "C" | "D" | "E" | "F"

<digit> ::= "0" | "1" | "2" | "3" | "4"
		  | "5" | "6" | "7" | "8" | "9"

<newline> ::= "\n"
```