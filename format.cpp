
#include <iostream>
using namespace std;
class Text;

// ... class definitions
class Word {
public:
	char word[1000];
	int length = 0;
	int xSpaces = 0;

	Word() {
		word[0] = 32;
		length = 0;
		xSpaces = 0;
	}
	Word(char xText[10000], int start, int end) {
		for (int i = start; i < end; i++) {
			word[i - start] = xText[i];
		}
		length = end - start;
		//word[length] = 0;
		xSpaces = 0;
	}
	void display() {
		for (int i = 0; i < length; i++) {
			cout << word[i];
		}
		cout << "\t\t" << length << "\t" << xSpaces << endl;
	}
};

class Line {
public:
	static const int MAX_LENGTH = 60;
	char line[1000];
	int length = 0;
	int xSpaces = 0;
	Word words[MAX_LENGTH];
	int nWords = 0;

	Line() {
		length = 0;
		xSpaces = 0;
		words[0] = Word();
		nWords = 0;
		//line[0] = 0;
	}
	Line(Word xWords[5000], int startWord, int endWord, bool lastLine) {
		int cLength = 0;
		int counter = 0;
		for (int i = startWord; i < endWord; i++) {
			words[counter++] = xWords[i];
		}
		nWords = counter;
		counter = 0;
		for (int i = 0; i < nWords; i++) {
			for (int j = 0; j < words[i].length; j++) {
				line[counter++] = words[i].word[j];
			}
			if (i != (nWords - 1)) {
				line[counter++] = 32;
				cLength = cLength + words[i].length + 1;
			}
			if (i == (nWords - 1)) {
				cLength = cLength + words[i].length;
				//line[counter + 1] = '\0';
			}
		}
		length = cLength;
		xSpaces = Line::MAX_LENGTH - cLength;  

		// ... prevent last line from being distributed
		if (lastLine == true) {
			xSpaces = -1;
		}

		distributeSpaces();

		constructLine(lastLine);
	}
	void distributeSpaces() {
		int spaces = 0;
		while (spaces <= xSpaces) {
			for (int w = nWords - 2; w >= 0; w--) {
				spaces++;
				if (spaces <= xSpaces) {
					words[w].xSpaces++;
				}
			}
		}
	}
	void constructLine(bool lastLine) {
		// initialize line 
		for (int i = 0; i < length + xSpaces + nWords; i++) {
			line[i] = 65;
		}
		int counter = 0;
		for (int i = 0; i < nWords; i++) {
			for (int j = 0; j < words[i].length; j++) {
				line[counter++] = words[i].word[j];
			}
			for (int s = 0; s < words[i].xSpaces; s++) {
				line[counter++] = 32;
			}
			if (i != nWords - 1) {
				line[counter++] = 32;
			}
		}
		if (!lastLine) {
			length = 60;
		}
	}
	void display() {
		for (int i = 0; i < length; i++) {
			cout << line[i];
		}
		cout << "\t" << length << "\t" << xSpaces << endl;
	}
};

class Text {
public:
	char text[100000];
	int length = 0;
	Word words[5000];
	int nWords = 0;
	Line lines[1000];
	int nLines = 0;

	Text() {
		int counter = 0;
		char c = 0;
		while ((c = getchar()) != EOF) {
			if (c == 10) {
				c = 32;
			}

			text[counter++] = c;

		}
		text[counter] = 65;
		length = counter;
	}

	void display() {
		for (int i = 0; text[i] != 0; i++) {
			cout << text[i];
		}
		cout << endl;
	}

	void condenseSpaces() {
		// ... get a copy
		static char copy[100000];
		int counter = 0;
		bool isSpace = false;
		for (int i = 0; i < length; i++) {
			if (isSpace == false && text[i] == 32) {
				isSpace = true;
				copy[counter++] = text[i];
			}
			if (text[i] != 32) {
				copy[counter++] = text[i];
				isSpace = false;
			}
		}
		// ... initialize text
		for (int i = 0; i < length; i++) {
			text[i] = 0;
		}
		// ... reconstruct text
		for (int i = 0; i < counter; i++) {
			if ((i == counter - 1) && copy[i] == 32) {
				length = counter - 1;
			}
			else {
				text[i] = copy[i];
				length = counter;
			}
		}
	}

	void split() {
		int counter = 0;
		int previousSpace = 0;
		for (int i = 0; i < length; i++) {
			if (text[i] == 32) {
				words[counter++] = Word(text, previousSpace, i);
				previousSpace = i + 1;
			}
			// ... last word
			if (i == length - 1) {
				words[counter++] = Word(text, previousSpace, i + 2);
			}
		}
		nWords = counter;
	}

	void linesFactory() {
		int startWord = 0;
		int endWord = nWords;
		int cLength = 0;
		int counter = 0;
		bool lastLine = false;
		for (int i = 0; i < nWords; i++) {
			cLength = cLength + words[i].length + 1;
			if ((cLength > Line::MAX_LENGTH + 1)) {
				endWord = i;
				lines[counter++] = Line(words, startWord, endWord, false);
				startWord = i;
				cLength = words[i].length + 1;
				endWord = nWords;
			}
			if ((i == (nWords - 1)) && (!(cLength > Line::MAX_LENGTH + 1))) {
				lines[counter++] = Line(words, startWord, nWords, true);
			}
		}
		nLines = counter;
	}
	void displayLines(bool numbers) {
		for (int l = 0; l < nLines; l++) {
			for (int c = 0; c < lines[l].length; c++) {
				if (lines[l].line[c] == '\0') {
					lines[l].line[c] = 10;
				}
			}
		}
		if (numbers) {
			for (int i = 0; i < nLines; i++) {
				lines[i].display();
			}
		}
		else {
			for (int i = 0; i < nLines; i++) {
				for (int j = 0; j < lines[i].length; j++) {
					cout << lines[i].line[j];
				}
				if (i != (nLines - 1)) {
					cout << endl;
				}
			}
			char t = 10;
			//cout << t;
		}
	}
};


// ... global data



int main()
{
	static Text text1;
	text1.condenseSpaces();
	text1.split();
	//text1.display();
	text1.linesFactory();
	/*for (int i = 0; i < text1.nLines; i++) {
		text1.lines[i].display();
	}
	for (int i = 0; i < text1.nLines; i++) {
		for (int j = 0; j < text1.lines[i].nWords; j++) {
			text1.lines[i].words[j].display();
		}
	}
	cout << endl;*/
	text1.displayLines(false);
}

