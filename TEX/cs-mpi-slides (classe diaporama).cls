\NeedsTeXFormat{LaTeX2e}%
\ProvidesClass{cs-mpi-slides}[2023-09-01 Classe de slides, v1.0]%
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{beamer}}%
\ProcessOptions*%
\LoadClass{beamer}
%
% \RequirePackage{lmodern}
\RequirePackage{MnSymbol}
\RequirePackage{graphicx}
\RequirePackage[french]{babel}
\RequirePackage{listings}
\RequirePackage{lipsum}
\RequirePackage{tikz}
\RequirePackage{xcolor}
\RequirePackage{tcolorbox}
\RequirePackage{parskip}
\RequirePackage{stmaryrd}
\RequirePackage{forest}
\RequirePackage[scale=1]{ccicons}
\RequirePackage{algorithm}
\RequirePackage{algpseudocode}
\RequirePackage{pifont,relsize}
\RequirePackage{fontawesome5}
\RequirePackage{multicol}
\RequirePackage{tcolorbox}
\tcbuselibrary{skins, theorems, breakable, listings}
\usetikzlibrary{arrows, automata}
%==============================
\colorlet{maincolor}{black!35!red} %choisir sa couleur
\definecolor{solcolor}{cmyk}{0,0,0,1}
\colorlet{lightmaincolor}{maincolor!30!white}
\colorlet{verylightmaincolor}{maincolor!10!white}
\colorlet{backmaincolor}{maincolor!0!white}
\colorlet{darkmaincolor}{maincolor!90!black}
\colorlet{deepmaincolor}{maincolor!50!black}
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
\uselanguage{French}
\languagepath{French}
\setbeamertemplate{caption}{\insertcaption}
\setbeamertemplate{headline}{}
% footline
\setbeamercolor{footline}{fg=white!60!black}
\setbeamertemplate{footline}{%
\hrulefill\par%
\hspace*{1ex}\raisebox{0ex}[1em][1ex]{\insertsection}%
\hfill%
\insertpagenumber\hspace*{1ex}%
}%
% pas de menu de navigation
\setbeamertemplate{navigation symbols}{}
% couleurs générales
% \setbeamercolor{title}{fg=verylightmaincolor, bg=white}
% \setbeamercolor{subtitle}{fg=white!60!black}
% page de titre
\setbeamertemplate{title page}{%
\begin{center}
\color{maincolor}%
\fontsize{32}{32}\bfseries\selectfont%
\inserttitle%
\\[4mm]
\color{white!60!black}%
\fontsize{16}{16}\bfseries\selectfont%
\insertsubtitle
\end{center}}
% blocks
\setbeamertemplate{blocks}[rounded][shadow]
% \setbeamertemplate{definitions}[numbered]
% \setbeamertemplate{theorems}[numbered]
% standard block
\setbeamerfont{block title}{family=\sffamily, series=\bfseries\small}
\setbeamercolor{block title}{fg=black, bg=black!15!white}
\setbeamercolor{block body}{fg=black, bg=black!5!white}
% example block
\setbeamerfont{block title example}{family=\sffamily, series=\bfseries\small}
\setbeamercolor{block title example}{fg=black, bg=black!15!white}
\setbeamercolor{block body example}{fg=black, bg=black!5!white}
% alerted block
\setbeamerfont{block title alerted}{family=\sffamily, series=\bfseries\small}
\setbeamercolor{block title alerted}{fg=black, bg=black!15!white}
\setbeamercolor{block body alerted}{fg=black, bg=black!5!white}

%
\renewcommand\thesection{\Roman{section}}
\AtBeginSection{\frame{\sectionpage}}
\defbeamertemplate{section page}{mysection}{%
\begin{center}
\fontsize{32}{32}\bfseries\selectfont\expandafter\protect\insertsectionhead
\end{center}
}
\setbeamertemplate{section page}[mysection]
%
% \theoremstyle{plain}
\setbeamerfont{theorem}{shape=\normalfont}
\newenvironment{defn}{\definition}{\enddefinition}
\newenvironment{thm}{\theorem}{\endtheorem}
\newenvironment{cor}{\corollary}{\endcorollary}
\newenvironment{lem}{\lemma}{\endlemma}
\newenvironment{demo}{\proof}{\endproof}
%
% \setbeamertemplate{blocks}[rounded][shadow=true]
\setbeamerfont{frametitle}{family=\sffamily, series=\bfseries}
\setbeamercolor{frametitle}{fg=black, bg=white}%
% \setbeamercolor{item}{fg=black}%
% \setbeamertemplate{itemize item}[triangle]
% \setbeamertemplate{enumerate item}[default]
\newenvironment<>{prop}[1][\undefined]{%
\begin{actionenv}#2%
\ifx#1\undefined\def\insertblocktitle{Proposition}%
\else\def\insertblocktitle{Proposition ({\em#1})}%
\fi%
\par\usebeamertemplate{block begin}%
}{\par\usebeamertemplate{block end}\end{actionenv}}%
%==============================
% questions
\newcounter{quest}
\newcounter{squest}
\newcounter{ssquest}
\newcommand{\question}{\stepcounter{quest}\setcounter{squest}{0}\setcounter{ssquest}{0}\par\medskip\noindent\textbf{\sffamily$\filledmedtriangleright$ Question \thequest.\;}}
\newcommand{\squestion}{\stepcounter{squest}\par\medskip\hangafter=0\hangindent=5mm$\triangleright$ \thequest.\thesquest.\;}
\newcommand{\ssquestion}{\stepcounter{ssquest}\par\medskip\hangafter=0\hangindent=10mm$\triangleright\triangleright$ \thequest.\thesquest.\thessquest.\;}
%==============================
\newcommand{\email}[1]{\lfoot{\sffamily\textcolor{maincolor}{\ccbyncsaeu\enspace #1}}}
\newcommand{\classe}[1]{\lhead{\sffamily\textcolor{maincolor}{#1}}}
\newcommand{\spec}[1]{\rhead{\sffamily\textcolor{maincolor}{#1}}}
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
  basicstyle=\footnotesize\ttfamily,
  commentstyle=\color{maincolor},
  showstringspaces=false,
  breaklines=true,
  numbersep=4mm,
  tabsize=2,
  language=C,
}
%
\newcommand{\code}[1]{\lstinline[basicstyle=\ttfamily, breaklines]{#1}}
\newcommand{\codec}[1]{\text{\textcolor{maincolor}{\code{#1}}}}