#include <iostream>
#include "utf8string.h"

int main(int argc, char *argv[]) {
  utf8string s1, s2, s3;
  s1 = u8"あいうえおかきくけこ";
  s2 = "１２３４５６７８９０ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ　！”＃＄％＆’（）＊＋，ー．／：；＜＝＞？＠［＼］＾＿｛｜｝〜;;";
  s3 = s1 + s2;

  std::cout << s1.getStdString() << std::endl;
  std::cout << s2.getStdString() << std::endl;
  std::cout << s3.getStdString() << std::endl;

  for (uinteger i = 0; i < s3.length(); i++) {
    std::cout << " i=" << i << " " << s3.at(i).getStdString() << std::endl;
  }

  s3.remove(2);
  std::cout << s3.getStdString() << std::endl;
  for (uinteger i = 0; i < s3.length(); i++) {
    std::cout << " i=" << i << " " << s3.at(i).getStdString() << std::endl;
  }

  //uint32_t options = E_CONVERT_HW_NUMBER;
  // uint32_t options = E_CONVERT_HW_CAPITAL_ALPHABET;
  // uint32_t options = E_CONVERT_HW_SMALL_ALPHABET;
  uint32_t options = E_CONVERT_HW_SYMBOL;
  // uint32_t options = E_CONVERT_HW_SYMBOL | E_CONVERT_HW_NUMBER | E_CONVERT_HW_CAPITAL_ALPHABET | E_CONVERT_HW_SMALL_ALPHABET;

  s3.toHalfWidth(options);
  std::cout << s3.getStdString() << std::endl;
  for (uinteger i = 0; i < s3.length(); i++) {
    printf("i=%2d [%s]\n", i, s3.at(i).getCString());
    //    std::cout << " i=" << i << " " << s3.at(i).getStdString() << std::endl;
  }

  return 0;
}
