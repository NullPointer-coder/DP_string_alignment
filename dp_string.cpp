/**
 * calculate the optimum alignment between two strings
 * @author Jingbo Wang
 * @version 7 April 2022
 */
#include <climits>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "matrix.h"
using namespace std;

/**
 * the recursive, memoized optimum function for computing the edit distance
 * between two strings
 * @param s the first string
 * @param i the index of the first string
 * @param t the second string
 * @param j the index of the second string
 * @param memo the memo table
 * @return the optimal alignment score of the two strings
 */
int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap);


/**
 * find and print the optimal alignment of the two strings
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 * @param gap the gap penalty
 */
void traceback(const Matrix<int>& memo, const string& s, const string& t,
                          int gap);

/**
 * dump the memo table to standard output
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 */
void print_memo(const Matrix<int>& memo, const string& s, const string& t);


int main(int argc, char** argv)
{
  if (argc != 6)
  {
    cerr << "Usage: " << argv[0] << " s1 s2 match mismatch gap" << endl;
    return 1;
  }

  // add a space to the beginning of each string to allow traceback
  string s = argv[1];
  s = ' ' + s;
  string t = argv[2];
  t = ' ' + t;

  int match = stoi(argv[3]);
  int mismatch = stoi(argv[4]);
  int gap = stoi(argv[5]);

  cout << "match: " << match << endl;
  cout << "mismatch: " << mismatch << endl;
  cout << "gap: " << gap << endl;

  Matrix<int> memo(s.size() + 1, t.size() + 1);
  for (size_t row = 0; row <= s.size(); row++)
  {
    for (size_t col = 0; col <= t.size(); col++)
    {
      memo.at(row, col) = INT_MAX;
    }
  }

  int score =
    opt(s, s.size() - 1, t, t.size() - 1, memo, match, mismatch, gap);

  cout << "The optimal alignment score between " << s << " and " << t <<
    " is " << score << endl;

  cout << endl << "The completed memo table: " << endl << endl;

  print_memo(memo, s, t);

  traceback(memo, s, t, gap);
  return 0;
}

int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap)
{
  if (i == 0 && j > 0)
  {
    memo.at(i, j) = opt(s, i, t, j - 1, memo, match, mismatch, gap) + gap;
    }
    else if (j == 0 && i > 0)
    {
      memo.at(i, j) = opt(s, i - 1, t, j, memo, match, mismatch, gap)
                                    + gap;
    }
   if (i == 0 && j == 0)
    {
      memo.at(i, j) = 0;
    }
    if (memo.at(i, j) == INT_MAX)
    {
      int value1 = 0;
      if (s.at(i) == t.at(j))
      {
        value1 = opt(s, i - 1, t, j - 1, memo, match, mismatch, gap)
                         + match;
      }
      else
      {
        value1 = opt(s, i - 1, t, j - 1, memo, match, mismatch, gap)
                          + mismatch;
      }
      memo.at(i, j) = max(value1,
                                       max(opt(s, i, t, j - 1, memo,
                                              match, mismatch, gap)+ gap,
                                              opt(s, i - 1, t, j, memo,
                                              match, mismatch, gap)  + gap));
  }
  return memo.at(i, j);
}

void traceback(const Matrix<int>& memo, const string& s, const string& t,
                           int gap)
 {
  string temp_s = s;
  string temp_t = t;
  size_t i = temp_s.size() - 1;
  size_t j = temp_t.size() - 1;

  while (i != 0 ||  j != 0)
  {
    if (i == 0 && j != 0)
    {
      temp_s.insert(i + 1, "-");
      j--;
    }
    else if (j == 0 && i != 0)
    {
      temp_t.insert(j + 1, "-");
      i--;
    }
    else if ( memo.at(i, j) - gap == memo.at(i - 1, j))
    {
      temp_t.insert(j + 1, "-");
      i--;
    }
    else if (memo.at(i, j) - gap == memo.at(i , j - 1))
    {
      temp_s.insert(i + 1, "-");
      j--;
    }
    else if (memo.at(i, j) - gap != memo.at(i - 1, j)
                  && memo.at(i, j) - gap != memo.at(i , j - 1))
    {
      i--;
      j--;
    }
  }
  cout << "The aligned strings:" << endl;
  cout  << temp_s.erase(0, 1) << endl;
  cout << temp_t.erase(0, 1) << endl;
 }

void print_memo(const Matrix<int>& memo, const string& s, const string& t)
{
  int field_width = 6;
  int left_label_width = 6;
  int left_index_width = 3;

  cout << right << setw(left_label_width) << ' ';
  for (size_t col = 0; col < t.size(); col++)
  {
    cout << setw(field_width) << t.at(col);
  }
  cout << endl;

  cout << setw(left_label_width) << ' ';
  for (size_t col = 0; col < t.size(); col++)
  {
    cout << setw(field_width) << col;
  }
  cout << endl;

  cout << setw(left_label_width) << '+';
  for (size_t col = 0; col < t.size(); col++)
  {
    cout << setw(field_width) << "---";
  }
  cout << endl;

  for (size_t row = 0; row < s.size(); row++)
  {
    cout << s.at(row) << setw(left_index_width) << row << " |";
    for (size_t col = 0; col < t.size(); col++)
    {
      if (memo.at(row, col) == INT_MAX)
      {
        cout << setw(field_width) << "inf";
      }
      else
      {
        cout << setw(field_width) << memo.at(row, col);
      }
    }
    cout << endl;
  }
}
