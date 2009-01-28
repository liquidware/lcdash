/*; Generated 05/07/2004 22:21:41 by MStoolsII 
; GM #12146312 
; Standard GM type Coolant Thermistor for Megasquirt 
; Computed Steinhart-Hart coefficients: A=0.00162549  B=0.00019531  C=3.00954e-07
;	 0 Fahrenheit = 25000 ohm
;	 100 Fahrenheit =  1800 ohm
;	 210 Fahrenheit =   185 ohm
; For use with  2490 ohm bias resistor installed at R7.
THERMFACTOR:*/

const int GM_thermfactor[256] PROGMEM={
	0	,//	0 - sensor failure, use limp home value.		
	408	,//	1	209	     10 ohms
	353	,//	2	178	     20 ohms
	323	,//	3	162	     30 ohms
	303	,//	4	151	     40 ohms
	288	,//	5	142	     50 ohms
	276	,//	6	136	     60 ohms
	266	,//	7	130	     70 ohms
	258	,//	8	126	     81 ohms
	251	,//	9	121	     91 ohms
	244	,//	10	118	    102 ohms
	238	,//	11	115	    112 ohms
	233	,//	12	112	    123 ohms
	228	,//	13	109	    134 ohms
	224	,//	14	106	    145 ohms
	220	,//	15	104	    156 ohms
	216	,//	16	102	    167 ohms
	212	,//	17	100	    178 ohms
	209	,//	18	98	    189 ohms
	206	,//	19	96	    200 ohms
	203	,//	20	95	    212 ohms
	200	,//	21	93	    223 ohms
	197	,//	22	92	    235 ohms
	194	,//	23	90	    247 ohms
	192	,//	24	89	    259 ohms
	190	,//	25	88	    271 ohms
	187	,//	26	86	    283 ohms
	185	,//	27	85	    295 ohms
	183	,//	28	84	    307 ohms
	181	,//	29	83	    320 ohms
	179	,//	30	82	    332 ohms
	177	,//	31	81	    345 ohms
	175	,//	32	80	    357 ohms
	174	,//	33	79	    370 ohms
	172	,//	34	78	    383 ohms
	170	,//	35	77	    396 ohms
	168	,//	36	76	    409 ohms
	167	,//	37	75	    423 ohms
	165	,//	38	74	    436 ohms
	164	,//	39	73	    450 ohms
	162	,//	40	72	    463 ohms
	161	,//	41	72	    477 ohms
	159	,//	42	71	    491 ohms
	158	,//	43	70	    505 ohms
	157	,//	44	69	    519 ohms
	155	,//	45	69	    534 ohms
	154	,//	46	68	    548 ohms
	153	,//	47	67	    563 ohms
	152	,//	48	66	    577 ohms
	150	,//	49	66	    592 ohms
	149	,//	50	65	    607 ohms
	148	,//	51	64	    622 ohms
	147	,//	52	64	    638 ohms
	146	,//	53	63	    653 ohms
	145	,//	54	63	    669 ohms
	143	,//	55	62	    685 ohms
	142	,//	56	61	    701 ohms
	141	,//	57	61	    717 ohms
	140	,//	58	60	    733 ohms
	139	,//	59	60	    750 ohms
	138	,//	60	59	    766 ohms
	137	,//	61	58	    783 ohms
	136	,//	62	58	    800 ohms
	135	,//	63	57	    817 ohms
	134	,//	64	57	    834 ohms
	133	,//	65	56	    852 ohms
	132	,//	66	56	    870 ohms
	131	,//	67	55	    887 ohms
	130	,//	68	55	    905 ohms
	130	,//	69	54	    924 ohms
	129	,//	70	54	    942 ohms
	128	,//	71	53	    961 ohms
	127	,//	72	53	    980 ohms
	126	,//	73	52	    999 ohms
	125	,//	74	52	   1018 ohms
	124	,//	75	51	   1038 ohms
	123	,//	76	51	   1057 ohms
	123	,//	77	50	   1077 ohms
	122	,//	78	50	   1097 ohms
	121	,//	79	49	   1118 ohms
	120	,//	80	49	   1138 ohms
	119	,//	81	48	   1159 ohms
	118	,//	82	48	   1180 ohms
	118	,//	83	48	   1202 ohms
	117	,//	84	47	   1223 ohms
	116	,//	85	47	   1245 ohms
	115	,//	86	46	   1267 ohms
	115	,//	87	46	   1289 ohms
	114	,//	88	45	   1312 ohms
	113	,//	89	45	   1335 ohms
	112	,//	90	45	   1358 ohms
	111	,//	91	44	   1382 ohms
	111	,//	92	44	   1405 ohms
	110	,//	93	43	   1429 ohms
	109	,//	94	43	   1454 ohms
	109	,//	95	43	   1478 ohms
	108	,//	96	42	   1503 ohms
	107	,//	97	42	   1529 ohms
	106	,//	98	41	   1554 ohms
	106	,//	99	41	   1580 ohms
	105	,//	100	40	   1606 ohms
	104	,//	101	40	   1633 ohms
	103	,//	102	40	   1660 ohms
	103	,//	103	39	   1687 ohms
	102	,//	104	39	   1715 ohms
	101	,//	105	39	   1743 ohms
	101	,//	106	38	   1771 ohms
	100	,//	107	38	   1800 ohms
	99	,//	108	37	   1829 ohms
	99	,//	109	37	   1859 ohms
	98	,//	110	37	   1889 ohms
	97	,//	111	36	   1919 ohms
	97	,//	112	36	   1950 ohms
	96	,//	113	36	   1981 ohms
	95	,//	114	35	   2013 ohms
	95	,//	115	35	   2045 ohms
	94	,//	116	34	   2078 ohms
	93	,//	117	34	   2111 ohms
	93	,//	118	34	   2145 ohms
	92	,//	119	33	   2179 ohms
	91	,//	120	33	   2213 ohms
	91	,//	121	33	   2248 ohms
	90	,//	122	32	   2284 ohms
	89	,//	123	32	   2320 ohms
	89	,//	124	31	   2357 ohms
	88	,//	125	31	   2394 ohms
	87	,//	126	31	   2432 ohms
	87	,//	127	30	   2471 ohms
	86	,//	128	30	   2510 ohms
	85	,//	129	30	   2549 ohms
	85	,//	130	29	   2590 ohms
	84	,//	131	29	   2631 ohms
	83	,//	132	29	   2672 ohms
	83	,//	133	28	   2715 ohms
	82	,//	134	28	   2758 ohms
	81	,//	135	27	   2801 ohms
	81	,//	136	27	   2846 ohms
	80	,//	137	27	   2891 ohms
	80	,//	138	26	   2937 ohms
	79	,//	139	26	   2984 ohms
	78	,//	140	26	   3031 ohms
	78	,//	141	25	   3080 ohms
	77	,//	142	25	   3129 ohms
	76	,//	143	25	   3179 ohms
	76	,//	144	24	   3230 ohms
	75	,//	145	24	   3282 ohms
	74	,//	146	24	   3335 ohms
	74	,//	147	23	   3389 ohms
	73	,//	148	23	   3444 ohms
	72	,//	149	22	   3500 ohms
	72	,//	150	22	   3557 ohms
	71	,//	151	22	   3615 ohms
	70	,//	152	21	   3675 ohms
	70	,//	153	21	   3735 ohms
	69	,//	154	21	   3797 ohms
	69	,//	155	20	   3860 ohms
	68	,//	156	20	   3924 ohms
	67	,//	157	20	   3989 ohms
	67	,//	158	19	   4056 ohms
	66	,//	159	19	   4124 ohms
	65	,//	160	18	   4194 ohms
	65	,//	161	18	   4265 ohms
	64	,//	162	18	   4337 ohms
	63	,//	163	17	   4412 ohms
	63	,//	164	17	   4487 ohms
	62	,//	165	17	   4565 ohms
	61	,//	166	16	   4644 ohms
	61	,//	167	16	   4725 ohms
	60	,//	168	15	   4808 ohms
	59	,//	169	15	   4893 ohms
	58	,//	170	15	   4980 ohms
	58	,//	171	14	   5069 ohms
	57	,//	172	14	   5160 ohms
	56	,//	173	14	   5253 ohms
	56	,//	174	13	   5349 ohms
	55	,//	175	13	   5447 ohms
	54	,//	176	12	   5547 ohms
	54	,//	177	12	   5650 ohms
	53	,//	178	12	   5756 ohms
	52	,//	179	11	   5865 ohms
	51	,//	180	11	   5976 ohms
	51	,//	181	10	   6090 ohms
	50	,//	182	10	   6208 ohms
	49	,//	183	10	   6329 ohms
	49	,//	184	9	   6453 ohms
	48	,//	185	9	   6581 ohms
	47	,//	186	8	   6712 ohms
	46	,//	187	8	   6848 ohms
	46	,//	188	8	   6987 ohms
	45	,//	189	7	   7130 ohms
	44	,//	190	7	   7278 ohms
	43	,//	191	6	   7431 ohms
	42	,//	192	6	   7589 ohms
	42	,//	193	5	   7751 ohms
	41	,//	194	5	   7919 ohms
	40	,//	195	4	   8092 ohms
	39	,//	196	4	   8272 ohms
	38	,//	197	4	   8457 ohms
	38	,//	198	3	   8649 ohms
	37	,//	199	3	   8848 ohms
	36	,//	200	2	   9055 ohms
	35	,//	201	2	   9268 ohms
	34	,//	202	1	   9490 ohms
	33	,//	203	1	   9721 ohms
	32	,//	204	0	   9960 ohms
	31	,//	205	0	  10209 ohms
	31	,//	206	-1	  10468 ohms
	30	,//	207	-1	  10738 ohms
	29	,//	208	-2	  11020 ohms
	28	,//	209	-2	  11313 ohms
	27	,//	210	-3	  11620 ohms
	26	,//	211	-3	  11941 ohms
	25	,//	212	-4	  12276 ohms
	24	,//	213	-5	  12628 ohms
	23	,//	214	-5	  12997 ohms
	22	,//	215	-6	  13384 ohms
	21	,//	216	-6	  13791 ohms
	20	,//	217	-7	  14219 ohms
	18	,//	218	-8	  14671 ohms
	17	,//	219	-8	  15148 ohms
	16	,//	220	-9	  15651 ohms
	15	,//	221	-9	  16185 ohms
	14	,//	222	-10	  16751 ohms
	13	,//	223	-11	  17352 ohms
	11	,//	224	-12	  17992 ohms
	10	,//	225	-12	  18675 ohms
	9	,//	226	-13	  19405 ohms
	7	,//	227	-14	  20187 ohms
	6	,//	228	-15	  21027 ohms
	4	,//	229	-15	  21931 ohms
	3	,//	230	-16	  22908 ohms
	1	,//	231	-17	  23966 ohms
	0	,//	232	-18	  25117 ohms
	-2	,//	233	-19	  26371 ohms
	-4	,//	234	-20	  27746 ohms
	-5	,//	235	-21	  29258 ohms
	-7	,//	236	-22	  30928 ohms
	-9	,//	237	-23	  32785 ohms
	-11	,//	238	-24	  34860 ohms
	-13	,//	239	-25	  37194 ohms
	-15	,//	240	-26	  39840 ohms
	-18	,//	241	-28	  42864 ohms
	-20	,//	242	-29	  46352 ohms
	-23	,//	243	-31	  50422 ohms
	-26	,//	244	-32	  55233 ohms
	-29	,//	245	-34	  61005 ohms
	-32	,//	246	-36	  68060 ohms
	-36	,//	247	-38	  76879 ohms
	-40	,//	248	-40	  88217 ohms
	-45	,//	249	-43	 103335 ohms
	-51	,//	250	-46	 124500 ohms
	-58	,//	251	-50	 156248 ohms
	-66	,//	252	-54	 209160 ohms
	-77	,//	253	-61	 314985 ohms
	-96	,//	254	-71	 632460 ohms
	0	,//	 255 - sensor failure, use limp home value.
};
