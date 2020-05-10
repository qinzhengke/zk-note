vscode中的markdown相关的代码片{#vscode_markdown_snippet}
=====================================================

\verbatim
//  ~/.config/Code/User/snippets/markdown.json
{
	// Place your snippets for markdown here. Each snippet is defined under a snippet name and has a prefix, body and 
	// description. The prefix is what is used to trigger the snippet and the body will be expanded and inserted. Possible variables are:
	// $1, $2 for tab stops, $0 for the final cursor position, and ${1:label}, ${2:another} for placeholders. Placeholders with the 
	// same ids are connected.
	// Example:
	// "Print to console": {
	// 	"prefix": "log",
	// 	"body": [
	// 		"console.log('$1');",
	// 		"$2"
	// 	],
	// 	"description": "Log output to console"
	// }
	"Inline function": {
		"prefix": "if",
		"body": [
			"\\f$ $0 \\f$"
		],
		"description": "inline formula"
	},
	"new line function": {
		"prefix": "nf",
		"body": [
			"\\f[",
			"$0",
			"\\f]"
		],
		"description": "new line formula"
	},
	"algin": {
		"prefix": "algined",
		"body": [
			"\\begin{aligned}",
			"$0",
			"\\end{aligned}"
		],
		"description": "new line formula"
	},
	"boldsymbol x": {
		"prefix": "bx",
		"body": [
			"\\boldsymbol{x}",
		],
		"description": "boldsymbol{x}"
	},
	"boldsymbol y": {
		"prefix": "by",
		"body": [
			"\\boldsymbol{y}",
		],
		"description": "boldsymbol{y}"
	},
	"boldsymbol A": {
		"prefix": "ba",
		"body": [
			"\\boldsymbol{A}",
		],
		"description": "boldsymbol{A}"
	},
	"boldsymbol H": {
		"prefix": "bh",
		"body": [
			"\\boldsymbol{H}",
		],
		"description": "boldsymbol{H}"
	},
	"boldsymbol J": {
		"prefix": "bj",
		"body": [
			"\\boldsymbol{J}",
		],
		"description": "boldsymbol{J}"
	},
	"boldsymbol f": {
		"prefix": "bf",
		"body": [
			"\\boldsymbol{f}",
		],
		"description": "boldsymbol{H}"
	},
	"boldsymbol D": {
		"prefix": "bd",
		"body": [
			"\\boldsymbol{D}",
		],
		"description": "boldsymbol{D}"
	},
	"boldsymbol I": {
		"prefix": "bi",
		"body": [
			"\\boldsymbol{I}",
		],
		"description": "boldsymbol{I}"
	},
	"boldsymbol 0": {
		"prefix": "b0",
		"body": [
			"\\boldsymbol{0}",
		],
		"description": "boldsymbol{0}"
	},
	"delta boldsymbol x": {
		"prefix": "dbx",
		"body": [
			"\\Delta\\boldsymbol{x}",
		],
		"description": "delta boldsymbol{x}"
	}
}
\endverbatim