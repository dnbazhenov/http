#include <cstring>
#include <iostream>

#include <httputil/char_traits/core_traits.h>
#include <httputil/char_traits/uri_traits.h>
#include <httputil/char_traits/field_traits.h>

static bool test_alpha[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_bit[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_char[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		true,		true,		true,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		true,		true,		true,		true,
	//	8	BS		9	HT		10	LF		11	VT
		true,		true,		true,		true,
	// 12	FF		13	CR		14	SO		15	SI
		true,		true,		true,		true,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		true,		true,		true,		true,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		true,		true,		true,		true,
	//	24	CAN		25	EM		26	SUB		27	ESC
		true,		true,		true,		true,
	//	28	FS		29	GS		30	RS		31	US
		true,		true,		true,		true,
	//	32	SP		33	!		34	"		35	#
		true,		true,		true,		true,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		true,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		true,		true,		true,		true,
	//	64	@		65	A		66	B		67	C
		true,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		true,
	//	92	\		93	]		94	^		95	_
		true,		true,		true,		true,
	//	96	`		97	a		98	b		99	c
		true,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		true,
	//	124	|		125	}		126	~		127	DEL
		true,		true,		true,		true,
};

static bool test_ctl[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		true,		true,		true,		true,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		true,		true,		true,		true,
	//	8	BS		9	HT		10	LF		11	VT
		true,		true,		true,		true,
	// 12	FF		13	CR		14	SO		15	SI
		true,		true,		true,		true,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		true,		true,		true,		true,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		true,		true,		true,		true,
	//	24	CAN		25	EM		26	SUB		27	ESC
		true,		true,		true,		true,
	//	28	FS		29	GS		30	RS		31	US
		true,		true,		true,		true,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		true,
};

static bool test_digit[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_hexdig[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_vchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		true,		true,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		true,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		true,		true,		true,		true,
	//	64	@		65	A		66	B		67	C
		true,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		true,
	//	92	\		93	]		94	^		95	_
		true,		true,		true,		true,
	//	96	`		97	a		98	b		99	c
		true,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		true,
	//	124	|		125	}		126	~		127	DEL
		true,		true,		true,		false,
};

static bool test_wsp[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		true,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		true,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_gen_delim[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		true,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		false,		false,		true,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		true,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		true,
	//	64	@		65	A		66	B		67	C
		true,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		true,
	//	92	\		93	]		94	^		95	_
		false,		true,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_sub_delim[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		false,
	//	36	$		37	%		38	&		39	'
		true,		false,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		false,		false,		false,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		false,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_reserved[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		true,
	//	36	$		37	%		38	&		39	'
		true,		false,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		false,		false,		true,
	//	48	0		49	1		50	2		51	3
		false,		false,		false,		false,
	//	52	4		53	5		54	6		55	7
		false,		false,		false,		false,
	//	56	8		57	9		58	:		59	;
		false,		false,		true,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		true,
	//	64	@		65	A		66	B		67	C
		true,		false,		false,		false,
	//	68	D		69	E		70	F		71	G
		false,		false,		false,		false,
	//	72	H		73	I		74	J		75	K
		false,		false,		false,		false,
	//	76	L		77	M		78	N		79	O
		false,		false,		false,		false,
	//	80	P		81	Q		82	R		83	S
		false,		false,		false,		false,
	//	84	T		85	U		86	V		87	W
		false,		false,		false,		false,
	//	88	X		89	Y		90	Z		91	[
		false,		false,		false,		true,
	//	92	\		93	]		94	^		95	_
		false,		true,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		false,		false,		false,
	//	100	d		101	e		102	f		103	g
		false,		false,		false,		false,
	//	104	h		105	i		106	j		107	k
		false,		false,		false,		false,
	//	108	l		109	m		110	n		111	o
		false,		false,		false,		false,
	//	112	p		113	q		114	r		115	s
		false,		false,		false,		false,
	//	116	t		117	u		118	v		119	w
		false,		false,		false,		false,
	//	120	x		121	y		122	z		123	{
		false,		false,		false,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_unreserved[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		false,
	//	44	,		45	-		46	.		47	/
		false,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool test_schar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		true,
	//	44	,		45	-		46	.		47	/
		false,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		false,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		false,		false,
};

static bool test_hchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		false,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool test_uchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		false,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool test_pchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		false,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		false,
	//	64	@		65	A		66	B		67	C
		true,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool test_qchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		false,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		true,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		false,		true,		false,		true,
	//	64	@		65	A		66	B		67	C
		true,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool test_obs_text[256] = {
		//	0	NUL		1	SOH		2	STX		3	ETX
			false,		false,		false,		false,
		//	4	EOT		5	ENQ		6	ACK		7	BEL
			false,		false,		false,		false,
		//	8	BS		9	HT		10	LF		11	VT
			false,		false,		false,		false,
		// 12	FF		13	CR		14	SO		15	SI
			false,		false,		false,		false,
		//	16	DLE		17	DC1		18	DC2		19	DC3
			false,		false,		false,		false,
		//	20	DC4		21	NAK		22	SYN		23	ETB
			false,		false,		false,		false,
		//	24	CAN		25	EM		26	SUB		27	ESC
			false,		false,		false,		false,
		//	28	FS		29	GS		30	RS		31	US
			false,		false,		false,		false,
		//	32	SP		33	!		34	"		35	#
			false,		false,		false,		false,
		//	36	$		37	%		38	&		39	'
			false,		false,		false,		false,
		//	40	(		41	)		42	*		43	+
			false,		false,		false,		false,
		//	44	,		45	-		46	.		47	/
			false,		false,		false,		false,
		//	48	0		49	1		50	2		51	3
			false,		false,		false,		false,
		//	52	4		53	5		54	6		55	7
			false,		false,		false,		false,
		//	56	8		57	9		58	:		59	;
			false,		false,		false,		false,
		//	60	<		61	=		62	>		63	?
			false,		false,		false,		false,
		//	64	@		65	A		66	B		67	C
			false,		false,		false,		false,
		//	68	D		69	E		70	F		71	G
			false,		false,		false,		false,
		//	72	H		73	I		74	J		75	K
			false,		false,		false,		false,
		//	76	L		77	M		78	N		79	O
			false,		false,		false,		false,
		//	80	P		81	Q		82	R		83	S
			false,		false,		false,		false,
		//	84	T		85	U		86	V		87	W
			false,		false,		false,		false,
		//	88	X		89	Y		90	Z		91	[
			false,		false,		false,		false,
		//	92	\		93	]		94	^		95	_
			false,		false,		false,		false,
		//	96	`		97	a		98	b		99	c
			false,		false,		false,		false,
		//	100	d		101	e		102	f		103	g
			false,		false,		false,		false,
		//	104	h		105	i		106	j		107	k
			false,		false,		false,		false,
		//	108	l		109	m		110	n		111	o
			false,		false,		false,		false,
		//	112	p		113	q		114	r		115	s
			false,		false,		false,		false,
		//	116	t		117	u		118	v		119	w
			false,		false,		false,		false,
		//	120	x		121	y		122	z		123	{
			false,		false,		false,		false,
		//	124	|		125	}		126	~		127	DEL
			false,		false,		false,		false,
		//	128 - 255
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
};

static bool test_ctext[256] = {
		//	0	NUL		1	SOH		2	STX		3	ETX
			false,		false,		false,		false,
		//	4	EOT		5	ENQ		6	ACK		7	BEL
			false,		false,		false,		false,
		//	8	BS		9	HT		10	LF		11	VT
			false,		true,		false,		false,
		// 12	FF		13	CR		14	SO		15	SI
			false,		false,		false,		false,
		//	16	DLE		17	DC1		18	DC2		19	DC3
			false,		false,		false,		false,
		//	20	DC4		21	NAK		22	SYN		23	ETB
			false,		false,		false,		false,
		//	24	CAN		25	EM		26	SUB		27	ESC
			false,		false,		false,		false,
		//	28	FS		29	GS		30	RS		31	US
			false,		false,		false,		false,
		//	32	SP		33	!		34	"		35	#
			true,		true,		true,		true,
		//	36	$		37	%		38	&		39	'
			true,		true,		true,		true,
		//	40	(		41	)		42	*		43	+
			false,		false,		true,		true,
		//	44	,		45	-		46	.		47	/
			true,		true,		true,		true,
		//	48	0		49	1		50	2		51	3
			true,		true,		true,		true,
		//	52	4		53	5		54	6		55	7
			true,		true,		true,		true,
		//	56	8		57	9		58	:		59	;
			true,		true,		true,		true,
		//	60	<		61	=		62	>		63	?
			true,		true,		true,		true,
		//	64	@		65	A		66	B		67	C
			true,		true,		true,		true,
		//	68	D		69	E		70	F		71	G
			true,		true,		true,		true,
		//	72	H		73	I		74	J		75	K
			true,		true,		true,		true,
		//	76	L		77	M		78	N		79	O
			true,		true,		true,		true,
		//	80	P		81	Q		82	R		83	S
			true,		true,		true,		true,
		//	84	T		85	U		86	V		87	W
			true,		true,		true,		true,
		//	88	X		89	Y		90	Z		91	[
			true,		true,		true,		true,
		//	92	\		93	]		94	^		95	_
			false,		true,		true,		true,
		//	96	`		97	a		98	b		99	c
			true,		true,		true,		true,
		//	100	d		101	e		102	f		103	g
			true,		true,		true,		true,
		//	104	h		105	i		106	j		107	k
			true,		true,		true,		true,
		//	108	l		109	m		110	n		111	o
			true,		true,		true,		true,
		//	112	p		113	q		114	r		115	s
			true,		true,		true,		true,
		//	116	t		117	u		118	v		119	w
			true,		true,		true,		true,
		//	120	x		121	y		122	z		123	{
			true,		true,		true,		true,
		//	124	|		125	}		126	~		127	DEL
			true,		true,		true,		false,
		//	128 - 255
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
};

static bool test_etagc[256] = {
		//	0	NUL		1	SOH		2	STX		3	ETX
			false,		false,		false,		false,
		//	4	EOT		5	ENQ		6	ACK		7	BEL
			false,		false,		false,		false,
		//	8	BS		9	HT		10	LF		11	VT
			false,		false,		false,		false,
		// 12	FF		13	CR		14	SO		15	SI
			false,		false,		false,		false,
		//	16	DLE		17	DC1		18	DC2		19	DC3
			false,		false,		false,		false,
		//	20	DC4		21	NAK		22	SYN		23	ETB
			false,		false,		false,		false,
		//	24	CAN		25	EM		26	SUB		27	ESC
			false,		false,		false,		false,
		//	28	FS		29	GS		30	RS		31	US
			false,		false,		false,		false,
		//	32	SP		33	!		34	"		35	#
			false,		true,		false,		true,
		//	36	$		37	%		38	&		39	'
			true,		true,		true,		true,
		//	40	(		41	)		42	*		43	+
			true,		true,		true,		true,
		//	44	,		45	-		46	.		47	/
			true,		true,		true,		true,
		//	48	0		49	1		50	2		51	3
			true,		true,		true,		true,
		//	52	4		53	5		54	6		55	7
			true,		true,		true,		true,
		//	56	8		57	9		58	:		59	;
			true,		true,		true,		true,
		//	60	<		61	=		62	>		63	?
			true,		true,		true,		true,
		//	64	@		65	A		66	B		67	C
			true,		true,		true,		true,
		//	68	D		69	E		70	F		71	G
			true,		true,		true,		true,
		//	72	H		73	I		74	J		75	K
			true,		true,		true,		true,
		//	76	L		77	M		78	N		79	O
			true,		true,		true,		true,
		//	80	P		81	Q		82	R		83	S
			true,		true,		true,		true,
		//	84	T		85	U		86	V		87	W
			true,		true,		true,		true,
		//	88	X		89	Y		90	Z		91	[
			true,		true,		true,		true,
		//	92	\		93	]		94	^		95	_
			true,		true,		true,		true,
		//	96	`		97	a		98	b		99	c
			true,		true,		true,		true,
		//	100	d		101	e		102	f		103	g
			true,		true,		true,		true,
		//	104	h		105	i		106	j		107	k
			true,		true,		true,		true,
		//	108	l		109	m		110	n		111	o
			true,		true,		true,		true,
		//	112	p		113	q		114	r		115	s
			true,		true,		true,		true,
		//	116	t		117	u		118	v		119	w
			true,		true,		true,		true,
		//	120	x		121	y		122	z		123	{
			true,		true,		true,		true,
		//	124	|		125	}		126	~		127	DEL
			true,		true,		true,		false,
		//	128 - 255
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
};

static bool test_qdtext[256] = {
		//	0	NUL		1	SOH		2	STX		3	ETX
			false,		false,		false,		false,
		//	4	EOT		5	ENQ		6	ACK		7	BEL
			false,		false,		false,		false,
		//	8	BS		9	HT		10	LF		11	VT
			false,		true,		false,		false,
		// 12	FF		13	CR		14	SO		15	SI
			false,		false,		false,		false,
		//	16	DLE		17	DC1		18	DC2		19	DC3
			false,		false,		false,		false,
		//	20	DC4		21	NAK		22	SYN		23	ETB
			false,		false,		false,		false,
		//	24	CAN		25	EM		26	SUB		27	ESC
			false,		false,		false,		false,
		//	28	FS		29	GS		30	RS		31	US
			false,		false,		false,		false,
		//	32	SP		33	!		34	"		35	#
			true,		true,		false,		true,
		//	36	$		37	%		38	&		39	'
			true,		true,		true,		true,
		//	40	(		41	)		42	*		43	+
			true,		true,		true,		true,
		//	44	,		45	-		46	.		47	/
			true,		true,		true,		true,
		//	48	0		49	1		50	2		51	3
			true,		true,		true,		true,
		//	52	4		53	5		54	6		55	7
			true,		true,		true,		true,
		//	56	8		57	9		58	:		59	;
			true,		true,		true,		true,
		//	60	<		61	=		62	>		63	?
			true,		true,		true,		true,
		//	64	@		65	A		66	B		67	C
			true,		true,		true,		true,
		//	68	D		69	E		70	F		71	G
			true,		true,		true,		true,
		//	72	H		73	I		74	J		75	K
			true,		true,		true,		true,
		//	76	L		77	M		78	N		79	O
			true,		true,		true,		true,
		//	80	P		81	Q		82	R		83	S
			true,		true,		true,		true,
		//	84	T		85	U		86	V		87	W
			true,		true,		true,		true,
		//	88	X		89	Y		90	Z		91	[
			true,		true,		true,		true,
		//	92	\		93	]		94	^		95	_
			false,		true,		true,		true,
		//	96	`		97	a		98	b		99	c
			true,		true,		true,		true,
		//	100	d		101	e		102	f		103	g
			true,		true,		true,		true,
		//	104	h		105	i		106	j		107	k
			true,		true,		true,		true,
		//	108	l		109	m		110	n		111	o
			true,		true,		true,		true,
		//	112	p		113	q		114	r		115	s
			true,		true,		true,		true,
		//	116	t		117	u		118	v		119	w
			true,		true,		true,		true,
		//	120	x		121	y		122	z		123	{
			true,		true,		true,		true,
		//	124	|		125	}		126	~		127	DEL
			true,		true,		true,		false,
		//	128 - 255
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
			true,		true,		true,		true,
};

static bool test_field_vchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		true,		true,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		true,		true,		true,		true,
	//	44	,		45	-		46	.		47	/
		true,		true,		true,		true,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		true,		true,
	//	60	<		61	=		62	>		63	?
		true,		true,		true,		true,
	//	64	@		65	A		66	B		67	C
		true,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		true,
	//	92	\		93	]		94	^		95	_
		true,		true,		true,		true,
	//	96	`		97	a		98	b		99	c
		true,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		true,
	//	124	|		125	}		126	~		127	DEL
		true,		true,		true,		false,
		//	128 - 255
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
		true,		true,		true,		true,
};

static bool test_tchar[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		true,		false,		true,
	//	36	$		37	%		38	&		39	'
		true,		true,		true,		true,
	//	40	(		41	)		42	*		43	+
		false,		false,		true,		true,
	//	44	,		45	-		46	.		47	/
		false,		true,		true,		false,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		true,		true,
	//	96	`		97	a		98	b		99	c
		true,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		true,		false,		true,		false,
};

static bool test_tchar68[256] = {
	//	0	NUL		1	SOH		2	STX		3	ETX
		false,		false,		false,		false,
	//	4	EOT		5	ENQ		6	ACK		7	BEL
		false,		false,		false,		false,
	//	8	BS		9	HT		10	LF		11	VT
		false,		false,		false,		false,
	// 12	FF		13	CR		14	SO		15	SI
		false,		false,		false,		false,
	//	16	DLE		17	DC1		18	DC2		19	DC3
		false,		false,		false,		false,
	//	20	DC4		21	NAK		22	SYN		23	ETB
		false,		false,		false,		false,
	//	24	CAN		25	EM		26	SUB		27	ESC
		false,		false,		false,		false,
	//	28	FS		29	GS		30	RS		31	US
		false,		false,		false,		false,
	//	32	SP		33	!		34	"		35	#
		false,		false,		false,		false,
	//	36	$		37	%		38	&		39	'
		false,		false,		false,		false,
	//	40	(		41	)		42	*		43	+
		false,		false,		false,		true,
	//	44	,		45	-		46	.		47	/
		false,		true,		true,		true,
	//	48	0		49	1		50	2		51	3
		true,		true,		true,		true,
	//	52	4		53	5		54	6		55	7
		true,		true,		true,		true,
	//	56	8		57	9		58	:		59	;
		true,		true,		false,		false,
	//	60	<		61	=		62	>		63	?
		false,		false,		false,		false,
	//	64	@		65	A		66	B		67	C
		false,		true,		true,		true,
	//	68	D		69	E		70	F		71	G
		true,		true,		true,		true,
	//	72	H		73	I		74	J		75	K
		true,		true,		true,		true,
	//	76	L		77	M		78	N		79	O
		true,		true,		true,		true,
	//	80	P		81	Q		82	R		83	S
		true,		true,		true,		true,
	//	84	T		85	U		86	V		87	W
		true,		true,		true,		true,
	//	88	X		89	Y		90	Z		91	[
		true,		true,		true,		false,
	//	92	\		93	]		94	^		95	_
		false,		false,		false,		true,
	//	96	`		97	a		98	b		99	c
		false,		true,		true,		true,
	//	100	d		101	e		102	f		103	g
		true,		true,		true,		true,
	//	104	h		105	i		106	j		107	k
		true,		true,		true,		true,
	//	108	l		109	m		110	n		111	o
		true,		true,		true,		true,
	//	112	p		113	q		114	r		115	s
		true,		true,		true,		true,
	//	116	t		117	u		118	v		119	w
		true,		true,		true,		true,
	//	120	x		121	y		122	z		123	{
		true,		true,		true,		false,
	//	124	|		125	}		126	~		127	DEL
		false,		false,		true,		false,
};

static bool check_table(const char *name, bool expected[], bool (*func)(char))
{
	bool okay = true;
	printf("Testing %s\n", name);

	for (unsigned int ch = 0; ch < 256; ch++) {
		if (func(ch) != expected[ch]) {
			printf("  Result doesn't match for '%c' (%d) %d/%d\n",
					ch, ch, func(ch), expected[ch]);
			okay = false;
		}
	}

	return okay;
}

int main(int argc, char *argv[])
{
	int n = 0;

	bool okay = true;

	// core traits
	okay = okay && check_table("is_alpha", test_alpha, httputil::char_traits::is_alpha);
	okay = okay && check_table("is_bit", test_bit, httputil::char_traits::is_bit);
	okay = okay && check_table("is_char", test_char, httputil::char_traits::is_char);
	okay = okay && check_table("is_ctl", test_ctl, httputil::char_traits::is_ctl);
	okay = okay && check_table("is_digit", test_digit, httputil::char_traits::is_digit);
	okay = okay && check_table("is_hexdig", test_hexdig, httputil::char_traits::is_hexdig);
	okay = okay && check_table("is_vchar", test_vchar, httputil::char_traits::is_vchar);
	okay = okay && check_table("is_wsp", test_wsp, httputil::char_traits::is_wsp);
	// uri traits
	okay = okay && check_table("is_gen_delim", test_gen_delim, httputil::char_traits::is_gen_delim);
	okay = okay && check_table("is_sub_delim", test_sub_delim, httputil::char_traits::is_sub_delim);
	okay = okay && check_table("is_reserved", test_reserved, httputil::char_traits::is_reserved);
	okay = okay && check_table("is_unreserved", test_unreserved, httputil::char_traits::is_unreserved);
	okay = okay && check_table("is_schar", test_schar, httputil::char_traits::is_schar);
	okay = okay && check_table("is_hchar", test_hchar, httputil::char_traits::is_hchar);
	okay = okay && check_table("is_uchar", test_uchar, httputil::char_traits::is_uchar);
	okay = okay && check_table("is_pchar", test_pchar, httputil::char_traits::is_pchar);
	okay = okay && check_table("is_qchar", test_qchar, httputil::char_traits::is_qchar);
	// field traits
	okay = okay && check_table("is_obs_text", test_obs_text, httputil::char_traits::is_obs_text);
	okay = okay && check_table("is_ctext", test_ctext, httputil::char_traits::is_ctext);
	okay = okay && check_table("is_etagc", test_etagc, httputil::char_traits::is_etagc);
	okay = okay && check_table("is_qdtext", test_qdtext, httputil::char_traits::is_qdtext);
	okay = okay && check_table("is_field_vchar", test_field_vchar, httputil::char_traits::is_field_vchar);
	okay = okay && check_table("is_tchar", test_tchar, httputil::char_traits::is_tchar);
	okay = okay && check_table("is_tchar68", test_tchar68, httputil::char_traits::is_tchar68);

	return okay ? 0 : 1;
}
