\NeedsTeXFormat{LaTeX2e}%
\ProvidesClass{cs-mpi}[2023-09-01, v1.0]%
\AtEndOfClass{\RequirePackage{microtype}}%
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}%
\ProcessOptions*%
\LoadClass{article}%
%==============================
\RequirePackage{fontspec}
\RequirePackage{unicode-math}
\RequirePackage[tt=false]{libertine}
\setmainfont[
  Ligatures=TeX,
  % Numbers=OldStyle,
  BoldFont={LinLibertine_RB.otf}, 
  ItalicFont={LinLibertine_RI.otf},
  BoldItalicFont={LinLibertine_RBI.otf}
]{LinLibertine_R.otf}
%
\RequirePackage{wasysym}
\RequirePackage{MnSymbol}
%
\RequirePackage{siunitx} % système d'unités SI
\sisetup{inter-unit-product = { } \cdot { }, output-decimal-marker = {,}}
\renewcommand{\si}{\SI[locale=FR,exponent-product=\cdot,inter-unit-product =\ensuremath{{}\cdot{}}, separate-uncertainty = true, multi-part-units=single]}
%
\RequirePackage[french]{babel}
\RequirePackage{xcolor}
\RequirePackage{colortbl}
\RequirePackage{graphicx,graphbox}
\RequirePackage{xspace}
\RequirePackage{marvosym}
\RequirePackage{eurosym}
\RequirePackage{wrapfig}
\RequirePackage{multicol}
\RequirePackage{multirow}
\RequirePackage{hhline}
\RequirePackage{float}
\RequirePackage[labelfont=sf,bf]{caption}
\RequirePackage[normalem]{ulem}
\RequirePackage{stmaryrd}
\RequirePackage[export]{adjustbox}
\RequirePackage{titlesec}
\RequirePackage{titletoc}
\RequirePackage[scale=1]{ccicons}
\RequirePackage{fontawesome5}
\RequirePackage{geometry}
\RequirePackage{lipsum}
\RequirePackage[nocheck]{fancyhdr}
\RequirePackage[inline]{enumitem}
\RequirePackage{tikz}
\usetikzlibrary{arrows.meta,positioning,automata,shapes}
\RequirePackage[linesnumbered, ruled, vlined, french]{algorithm2e}
%
\RequirePackage{tcolorbox}
\tcbuselibrary{skins, theorems, breakable, listings}
\usetikzlibrary{arrows, automata}
%
\RequirePackage[hyphens]{url}
\RequirePackage{sidenotes}
\RequirePackage{ifthen,verbatim}
\RequirePackage{subcaption}
\RequirePackage{todonotes}
%==============================
\colorlet{maincolor}{black!35!red} %choisir sa couleur
\definecolor{solcolor}{cmyk}{0,0,0,1}
\colorlet{lightmaincolor}{maincolor!30!white}
\colorlet{verylightmaincolor}{maincolor!10!white}
\colorlet{backmaincolor}{maincolor!0!white}
\colorlet{darkmaincolor}{maincolor!90!black}
\colorlet{deepmaincolor}{maincolor!50!black}%==============================
%==============================
% caractères spéciaux
\lstset{literate =
  {á}{{\'a}}1 {é}{{\'e}}1 {í}{{\'i}}1 {ó}{{\'o}}1 {ú}{{\'u}}1
  {Á}{{\'A}}1 {É}{{\'E}}1 {Í}{{\'I}}1 {Ó}{{\'O}}1 {Ú}{{\'U}}1
  {à}{{\`a}}1 {è}{{\`e}}1 {ì}{{\`i}}1 {ò}{{\`o}}1 {ù}{{\`u}}1
  {À}{{\`A}}1 {È}{{\'E}}1 {Ì}{{\`I}}1 {Ò}{{\`O}}1 {Ù}{{\`U}}1
  {ä}{{\"a}}1 {ë}{{\"e}}1 {ï}{{\"i}}1 {ö}{{\"o}}1 {ü}{{\"u}}1
  {Ä}{{\"A}}1 {Ë}{{\"E}}1 {Ï}{{\"I}}1 {Ö}{{\"O}}1 {Ü}{{\"U}}1
  {â}{{\^a}}1 {ê}{{\^e}}1 {î}{{\^i}}1 {ô}{{\^o}}1 {û}{{\^u}}1
  {Â}{{\^A}}1 {Ê}{{\^E}}1 {Î}{{\^I}}1 {Ô}{{\^O}}1 {Û}{{\^U}}1
  {Ã}{{\~A}}1 {ã}{{\~a}}1 {Õ}{{\~O}}1 {õ}{{\~o}}1
  {œ}{{\oe}}1 {Œ}{{\OE}}1 {æ}{{\ae}}1 {Æ}{{\AE}}1 {ß}{{\ss}}1
  {ű}{{\H{u}}}1 {Ű}{{\H{U}}}1 {ő}{{\H{o}}}1 {Ő}{{\H{O}}}1
  {ç}{{\c c}}1 {Ç}{{\c C}}1 {ø}{{\o}}1 {å}{{\r a}}1 {Å}{{\r A}}1
  {€}{{\euro}}1 {£}{{\pounds}}1 {«}{{\guillemotleft}}1
  {»}{{\guillemotright}}1 {ñ}{{\~n}}1 {Ñ}{{\~N}}1 {¿}{{?`}}1
}
%
\lstset{
  basicstyle=\linespread{0.9}\ttfamily,
  commentstyle=\color{maincolor},
  showstringspaces=false,
  breaklines=true,
  numbersep=4mm,
  tabsize=2,
  language=C,
}
%
\lstnewenvironment{ocaml}{\lstset{language=caml}}{}
\lstnewenvironment{python}{\lstset{language=python}}{}
\lstnewenvironment{C}{\lstset{language=C}}{}
\newcommand{\code}[1]{\lstinline[basicstyle=\ttfamily, breaklines]{#1}}
\newcommand{\codec}[1]{\text{\textcolor{maincolor}{\code{#1}}}}
%==============================
%==============================
% notations diverses
\newcommand{\B}{\ensuremath{\mathbb{B}}\xspace}
\newcommand{\N}{\ensuremath{\mathbb{N}}\xspace}
\newcommand{\NN}{\ensuremath{\mathbb{N}}\xspace}
\newcommand{\Z}{\ensuremath{\mathbb{Z}}\xspace}
\newcommand{\Q}{\ensuremath{\mathbb{Q}}\xspace}
\newcommand{\R}{\ensuremath{\mathbb{R}}\xspace}
\newcommand{\bigO}{\ensuremath{\mathcal{O}}\xspace}
\newcommand{\pset}[1]{\ensuremath{{\mathcal P}(#1)}\xspace}
\newcommand{\ff}{\ensuremath{{\mathcal F}}\xspace}
\newcommand{\trm}[1]{\textsf{#1}}
\newcommand{\eps}{\ensuremath{\varepsilon}\xspace}
\newcommand{\miroir}[1]{\ensuremath{\overline{#1}}\xspace}
\newcommand{\set}[1]{\ensuremath{\{#1\}}\xspace}
\newcommand{\ceil}[1]{\ensuremath{\lceil#1\rceil}\xspace}
\newcommand{\cmp}[2]{\ensuremath{#1\mathrel{<?}#2}\xspace}
\newcommand{\intervalle}[2]{\ensuremath{\left[#1,#2\right[}\xspace}%}}
\newcommand{\cintervalle}[2]{\ensuremath{\left[#1,#2\right]}\xspace}
\newcommand{\ocintervalle}[2]{\ensuremath{\left]#1,#2\right]}\xspace}
\newcommand{\oointervalle}[2]{\ensuremath{\left]#1,#2\right[}\xspace}
\newcommand{\proba}[1]{\ensuremath{\mathbb{P}(#1)}\xspace}
\newcommand{\expect}[1]{\ensuremath{\mathbb{E}(#1)}\xspace}
\newcommand{\xto}[1]{\ensuremath{\xrightarrow{#1}}\xspace}
\newcommand{\vrai}{\textsf{V}\xspace}
\newcommand{\faux}{\textsf{F}\xspace}
\newcommand{\lrbracket}[1]{\llbracket #1 \rrbracket}
\newcommand{\interp}[2]{#2(#1)\xspace}
% ref-like macros
\newcommand{\refsec}[1]{section~\ref{sec:#1}}
\newcommand{\refsecp}[1]{section~\ref{sec:#1} page~\pageref{sec:#1}}
\newcommand{\reffig}[1]{figure~\ref{fig:#1}}
\newcommand{\reffigp}[1]{figure~\ref{fig:#1} page~\pageref{fig:#1}}
\newcommand{\refpgm}[1]{programme~\ref{pgm:#1}}
\newcommand{\refpgmp}[1]{programme~\ref{pgm:#1} page~\pageref{pgm:#1}}
\newcommand{\refintf}[1]{interface~\ref{pgm:#1}}
\newcommand{\refintfp}[1]{interface~\ref{pgm:#1} page~\pageref{pgm:#1}}
\newcommand{\refchap}[1]{chapitre~\ref{chap:#1}}
\newcommand{\refchapp}[1]{chapitre~\ref{chap:#1} page~\pageref{chap:#1}}
\newcommand{\refprop}[1]{propriété~\ref{prop:#1}}
\newcommand{\refpropp}[1]{propriété~\ref{prop:#1} page~\pageref{prop:#1}}
\newcommand{\refdefn}[1]{définition~\ref{defn:#1}}
\newcommand{\refdefnp}[1]{définition~\ref{defn:#1} page~\pageref{defn:#1}}
\newcommand{\refthm}[1]{théorème~\ref{thm:#1}}
\newcommand{\refthmp}[1]{théorème~\ref{thm:#1} page~\pageref{thm:#1}}
\newcommand{\refex}[1]{exemple~\ref{ex:#1}}
\newcommand{\refexp}[1]{exemple~\ref{ex:#1} page~\pageref{ex:#1}}
% C
\newcommand{\void}{\oc{void}\index{void@\texttt{void}}\xspace}
\newcommand{\sizeof}{\oc{sizeof}\index{sizeof@\texttt{sizeof}}\xspace}
\newcommand{\malloc}{\oc{malloc}\indexcbibfun{stdlib}{malloc}\xspace}
\newcommand{\calloc}{\oc{calloc}\indexcbibfun{stdlib}{calloc}\xspace}
\newcommand{\rand}{\oc{rand}\indexcbibfun{stdlib}{rand}\xspace}
\newcommand{\free}{\oc{free}\indexcbibfun{stdlib}{free}\xspace}
\newcommand{\NULL}{\oc{NULL}\index{NULL@\texttt{NULL}}\xspace}
\newcommand{\addressof}{\oc{&}\index{\&@\texttt{\&}}\xspace}
% arbre binaires
\newcommand{\Empty}{\texttt{E}}
\newcommand{\Node}[3]{\ensuremath{\texttt{N}(#1,#2,#3)}}
% relations
\newcommand{\relop}[1][]{\ensuremath{{\mathcal R}_{#1}}\xspace}
\newcommand{\rel}[3][]{\ensuremath{#2\mathrel{\relop[#1]}#3}\xspace}
% listes
\newcommand{\nil}{\texttt{[]}\xspace}
\newcommand{\cons}[2]{#1\om{::}#2\xspace}
%
\newcommand{\longueur}[1]{\ensuremath{|#1|}\xspace}
\newcommand{\concat}[2]{\ensuremath{#1\cdot #2}\xspace}
\newcommand{\rev}[1]{\ensuremath{\overline{\,#1\,}}\xspace}
\newcommand{\app}{\texttt{~}}
\newcommand{\tri}[1]{\ensuremath{#1^\dagger}\xspace}
%
\renewcommand{\leq}{\leqslant}
\renewcommand{\le}{\leqslant}
\renewcommand{\geq}{\geqslant}
\renewcommand{\ge}{\geqslant}
%
\newcommand{\taille}[1]{\ensuremath{|#1|}}
\newcommand{\LSPACE}{\trm{L}\xspace}
\renewcommand{\P}{\trm{P}\xspace}
\newcommand{\NP}{\trm{NP}\xspace}
\newcommand{\coNP}{\trm{co-NP}\xspace}
\newcommand{\PSPACE}{\trm{PSPACE}\xspace}
\newcommand{\EXP}{\trm{EXPTIME}\xspace}
\newcommand{\polyred}{\mathrel{\leqslant_{\P}}}
\newcommand{\SAT}{\trm{SAT}\xspace}
\newcommand{\kSAT}[1]{\trm{#1SAT}\xspace}
\newcommand{\MAXSAT}{\trm{MAXSAT}\xspace}
\newcommand{\MAXkSAT}[1]{\trm{MAX#1SAT}\xspace}
\newcommand{\kCOLOR}[1]{\trm{#1COLOR}\xspace}
\newcommand{\DHAMPATH}{\trm{$d$HAM-PATH}\xspace}
\newcommand{\UHAMPATH}{\trm{$u$HAM-PATH}\xspace}
\newcommand{\DHAMCYCLE}{\trm{$d$HAM-CYCLE}\xspace}
\newcommand{\UHAMCYCLE}{\trm{$u$HAM-CYCLE}\xspace}
\newcommand{\TSP}{\trm{TSP}\xspace}
\newcommand{\kTSP}[1]{\trm{#1TSP}\xspace}
\newcommand{\dom}{\mathrm{dom}}
% Langages formels
\newcommand{\Auto}[2][A]{\ensuremath\mathcal{#1}_{#2}}
\newcommand{\KStar}[1]{\ensuremath {#1}^{*}}
\newcommand{\Omeg}[1]{\ensuremath {#1}^{\omega}}
\newcommand{\Inft}[1]{\ensuremath {#1}^{\infty}}
\newcommand{\SigmaStar}{\KStar{\Sigma}}
\newcommand{\Mirror}[1]{\ensuremath {#1}^{\textsf{R}}}
\newcommand{\Powerset}[1]{\ensuremath\mathcal{P}(#1)}
\newcommand{\Powersetf}[1]{\ensuremath\mathcal{P}_f(#1)}
\newcommand{\Rat}{\ensuremath\textsf{Rat}}
\newcommand{\Reg}{\ensuremath\textsf{Reg}}
\newcommand{\Rec}{\ensuremath\textsf{Rec}}
\newcommand{\Det}{\ensuremath\textsf{det}}
\newcommand{\Dete}{\ensuremath\textsf{det}^{\varepsilon}}
\newcommand{\Rme}{\ensuremath\textsf{rm}^{\varepsilon}}
\newcommand{\Thompson}{\ensuremath\textsf{th}}
\newcommand{\ATrans}[1]{\ensuremath \overset{#1}{\longrightarrow} }
\newcommand{\APath}[2]{\ensuremath \overset{#2}{\longrightarrow^{*}}_{#1}}
\newcommand{\Lang}[1]{\ensuremath L_{#1}}
\newcommand{\Compl}[1]{\ensuremath \overline{#1}}
\newcommand{\LangOf}[1]{\ensuremath\mathcal{L}(#1)}
\newcommand{\BlueCirc}[1]{\tikz[baseline=(x.base)]{\node[text centered,circle,fill=darkmaincolor,text=white,inner sep=1pt](x){#1}}}
\newcommand{\First}{\ensuremath \textsf{P}}
\newcommand{\Last}{\ensuremath \textsf{S}}
\newcommand{\Follow}{\ensuremath \textsf{Follow}}
\newcommand{\Fact}{\ensuremath \textsf{F}}
\newcommand{\NFact}{\ensuremath \textsf{N}}
\newcommand{\Loc}{\ensuremath \textsf{Loc}}
\newcommand{\Mark}{\ensuremath \textsf{Mark}}
\newcommand{\Del}{\ensuremath \textsf{Del}}
%==============================
% géométrie
\geometry{%
  hoffset=0mm, 
  voffset=0mm,%
  paperwidth=210mm, 
  left=20mm, 
  right=20mm,% width = paperwidth - left - right
  paperheight=297mm, 
  top=10mm, 
  bottom=10mm,% height = paperheight - top - bottom
  % marginparsep=0mm, marginparwidth=0mm,% textwidth = width - marginparsep - marginparwidth
  headheight=5mm, 
  headsep=5mm, 
  footskip=10mm, 
  includehead, 
  includefoot,% textheight = height - headheight - headsep - footskip si les options includehead et includefoot sont présentes
}%
%==============================
% entête et pied de page
%-------------------------
\pagestyle{fancy}
% \setlength{\parindent}{0mm}
\renewcommand{\headrulewidth}{.5pt}
\renewcommand{\footrulewidth}{.5pt}
\renewcommand{\headrule}{{\color{maincolor}\hrule width\headwidth height\headrulewidth \vskip-\headrulewidth}}
\renewcommand{\footrule}{{\color{maincolor}\vskip-\footruleskip\vskip-\footrulewidth\hrule width\headwidth height\footrulewidth\vskip\footruleskip}}
%-------------------------
\newcommand{\uleft}[1]{\lhead{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\ucent}[1]{\chead{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\uright}[1]{\rhead{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\dleft}[1]{\lfoot{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\dcent}[1]{\cfoot{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\dright}[1]{\rfoot{\sffamily\textcolor{maincolor}{#1}}}
\uleft{}
\ucent{}
\uright{}
\dleft{}
\dcent{}
\dright{}
%==============================
\setcounter{tocdepth}{2}
\newcommand{\marginsecnumber}[1]{\makebox[0pt][r]{#1\hspace{1ex}}}
%==============================
% puces des listes
\setlist{font=\color{maincolor}\sffamily\bfseries}
\setlist[itemize,1]{label=$\filleddiamond$}
\setlist[itemize,2]{label=$\smalldiamond$}
\setlist[itemize,3]{label=$\diamonddot$}
%==============================
% title
\renewcommand{\title}[1]{\noindent\textbf{\color{maincolor}\sffamily\Huge #1}\par\bigskip}
%-------------------------
% section
\titleformat{\section}[block]{\huge\sffamily\bfseries\color{maincolor}}{\marginsecnumber\thesection}{0pt}{}
\titlespacing{\section}{0pt}{*2.5}{.5ex plus .5ex}
%-------------------------
% subsection
\titleformat{\subsection}{\Large\sffamily\bfseries\color{maincolor}}{\marginsecnumber\thesubsection}{0pt}{}
\titlespacing{\subsection}{0pt}{*1.5}{.5ex plus .5ex}
%-------------------------
% subsubsection
\titleformat{\subsubsection}{\large\sffamily\bfseries\color{maincolor}}{\marginsecnumber\thesubsubsection}{0pt}{}
%-------------------------
% paragraphs
\renewenvironment{paragraph}[1]%
{\par\smallskip\noindent\textcolor{maincolor}{\bfseries #1.}\xspace}%
{\hfill$\medtriangleleft$\par\medskip}
%
\newenvironment{exemp}%
{\par\smallskip\noindent\textcolor{maincolor}{\bfseries Exemple}\xspace}%
{\hfill$\medtriangleleft$\par\medskip}
%
\newtheorem{exempinl}{\color{maincolor}Exemple}
\renewcommand{\theexempinl}{\arabic{exempinl}.}
%
\newtheorem{definline}{\color{maincolor}Définition}
\renewcommand{\thedefinline}{\arabic{definline}.}
%-------------------------
\newcounter{exo}
\newcounter{quest}
\newcounter{squest}
\newcounter{ssquest}
\setcounter{exo}{0}
\setcounter{quest}{0}
\setcounter{quest}{0}
\setcounter{quest}{0}
% exercices
\titleclass{\exo}{straight}[\section]
\titleformat{\exo}[block]{\textcolor{maincolor}{\sffamily\bfseries{\huge Exercice \theexo}}}{}{0pt}{}
\titlespacing{\exo}{0pt}{2.0ex plus .1ex minus .2ex}{1.0ex plus .1ex minus .2ex}
\newcommand{\exerc}{\setcounter{quest}{0}\exo{}}
% question
\newcommand{\question}{%
  \stepcounter{quest}%
  \setcounter{squest}{0}%
  \setlength{\leftskip}{0em}%
  \par\medskip\noindent%
  \textbf{Question \thequest.\hspace*{2\wordsep}}%
}
% sous-question
\newcommand{\squestion}{%
  \stepcounter{squest}%
  \setcounter{ssquest}{0}%
  % \setlength{\leftskip}{1em}%
  \par\noindent%
  \textbf{\textcolor{maincolor}{$\Box$} \thequest.\thesquest.\hspace*{2\wordsep}}%
}
% sous-sous-question
\newcommand{\ssquestion}{%
  \stepcounter{ssquest}%
  % \setlength{\leftskip}{2em}%
  \par\noindent%
  \textbf{\textcolor{maincolor}{$\rightslice$} \thequest.\thesquest.\thessquest.\hspace*{2\wordsep}}%
}
%==============================
% environnements
% définition
\newtcbtheorem[auto counter]{tcbdefn}{\faDownload\enspace Définition}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
  %% enhanced,
  breakable,
  %% colframe=lightmaincolor,colback=black,colbacktitle=white,
  %% fonttitle={\sffamily\bfseries\color{darkmaincolor}},
  %% separator sign dash,
}{defn}
\newenvironment{defn}[2][]{\begin{tcbdefn}{#2}{#1}}{\end{tcbdefn}}
% théorème
\newtcbtheorem[auto counter]{tcbthm}{\faCog\enspace Théorème}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
}{thm}
\newenvironment{thm}[2][]{\begin{tcbthm}{#2}{#1}}{\end{tcbthm}}
% proposition
\newtcbtheorem[auto counter]{tcbprop}{Proposition}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
}{prop}
\newenvironment{prop}[2][]{\begin{tcbprop}{#2}{#1}}{\end{tcbprop}}
% lemme
\newtcbtheorem[auto counter]{tcblem}{Lemme}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
}{thm}
\newenvironment{lem}[2][]{\begin{tcblem}{#2}{#1}}{\end{tcblem}}
% corolaire
\newtcbtheorem[auto counter]{tcbcor}{Corolaire}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
}{thm}
\newenvironment{cor}[2][]{\begin{tcbcor}{#2}{#1}}{\end{tcbcor}}
% exemple
\newtcbtheorem[auto counter]{tcbexemple}{Exemple}{%
  enhanced,
  breakable,
  colframe=verylightmaincolor,
  colback=backmaincolor,
  top=4mm,
  fonttitle={\sffamily\bfseries\color{black}},
  separator sign dash,
  attach boxed title to top left={yshift=-3mm},
  boxed title style={colframe=verylightmaincolor, colback=verylightmaincolor},
}{ex}
\newenvironment{exemple}[2][]{\begin{tcbexemple}{#2}{#1}}{\end{tcbexemple}}
% démonstration
\newenvironment{demo}%
  {\noindent\emph{Démonstration.}\xspace}%
  {\hfill$\Box$\par\medskip}
% remarque
\newenvironment{rem}%
  {\par\smallskip\noindent\textcolor{maincolor}{\bfseries Remarque.}\xspace}%
  {\hfill$\medtriangleleft$\par\medskip}
%==============================
% algorithmes
\DontPrintSemicolon
\SetKwInput{Ent}{Entrée}
\SetKwInput{Out}{Sortie}
\SetKwProg{Fn}{fonction}{}{}
\SetKwIF{If}{ElseIf}{Else}{si}{alors}{sinon si}{sinon}{fin si}
\SetKwFor{While}{pour}{faire}{}%
\SetKwFor{Repeat}{tant que}{faire}{}%
\SetKw{Ret}{renvoyer}%
%==============================