/* Copyright 2016 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.engedu.anagrams;

import java.io.BufferedReader;
import java.util.Arrays;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Random;

public class AnagramDictionary {

  private static final int MIN_NUM_ANAGRAMS = 5;
  private static final int DEFAULT_WORD_LENGTH = 3;
  private static final int MAX_WORD_LENGTH = 7;
  private static int wordLength = DEFAULT_WORD_LENGTH;
  private Random random = new Random();
  private ArrayList<String> wordList; // stores words of the dictionary
  private HashSet<String> wordSet; // stores words of the dictionary (for some optimizations)
  private HashMap<String,ArrayList<String>> lettersToWord;
                          // maps word of sorted letters to all its anagrams
  private HashMap<Integer,ArrayList<String>> sizeToWords;
  public AnagramDictionary(Reader reader) throws IOException {
    BufferedReader in = new BufferedReader(reader);
    lettersToWord=new HashMap<>();
    wordSet=new HashSet<>();
    sizeToWords = new HashMap<>();
    wordList = new ArrayList<>();

    String line;
    while ((line = in.readLine()) != null) {
      String word = line.trim();
      // adding to sizeToWord
      Integer size = Integer.valueOf(word.length());

      if(sizeToWords.containsKey(size)) {
        sizeToWords.get(size).add(word);
      }else {
        ArrayList<String> arr=new ArrayList<>();
        arr.add(word);
        sizeToWords.put(size,arr);
      }
      String Sorted=sortLetters(word);
      wordList.add(word);
      // adding to lettersToWord
      if(lettersToWord.containsKey(Sorted)) {
        lettersToWord.get(Sorted).add(word);
      }else {
        ArrayList<String> arr=new ArrayList<>();
        arr.add(word);
        lettersToWord.put(Sorted,arr);
      }
      wordSet.add(word);
    }
  }

  public boolean isGoodWord(String word, String base) {
    boolean isFound=wordSet.contains(word);
    return isFound && !word.contains(base);
  }

  public List<String> getAnagrams(String targetWord) {
    ArrayList<String> result = new ArrayList<String>();
    String Sorted = sortLetters(targetWord);
    if(lettersToWord.get(Sorted)!=null) {
      result = lettersToWord.get(Sorted);
    }
    return result;
  }

  private String sortLetters(String word) { // sorting the String
    char[] wordArray = word.toCharArray();
    Arrays.sort(wordArray);
    return new String(wordArray);
  }

  public List<String> getAnagramsWithOneMoreLetter(String word) {
    ArrayList<String> result = new ArrayList<String>();
    for(char i='a';i<='z';i++){
      String Sorted=sortLetters(word+i);
      List<String> NewAnagrams=getAnagrams(Sorted);
      for (String s: NewAnagrams) {
        if(!s.contains(word))
          result.add(s);
      }
    }
    return result;
  }

  public String pickGoodStarterWord() {
    int size = sizeToWords.get(wordLength).size();
    ArrayList<String> words = sizeToWords.get(wordLength);
    if(wordLength < MAX_WORD_LENGTH) {
        wordLength++;
    }
    int index = random.nextInt(size);
    while(getAnagramsWithOneMoreLetter(words.get(index)).size() < MIN_NUM_ANAGRAMS) {
      index = random.nextInt(size);
    }
    return words.get(index);
  }
}
