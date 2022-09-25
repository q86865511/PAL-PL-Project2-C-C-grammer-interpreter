# PAL-PL-Project2-C-C++-grammer-interpreter
一款類C語言文法檢查直譯器，能夠判斷是否符合文法，以及儲存變數或是函式名稱、內容與型別。

# 簡易版C語言Interpreter
使用語言:C++

編譯器:VisualStudioCode

# 介紹
此程式為中原大學三下PL(程式語言)project2

# 功能
1.可以根據輸入的字串 判斷是否為正確的C語言文法
(基本上大部分的C文法都能接受)

2.程式開始前要先隨便輸入一個數字 然後按enter後才算開始執行

3.程式結束以輸入 Done(); 為結束

4.可接受 變數宣告.function宣告.陣列宣告及運算.條件式.迴圈等等運算能力

5.提供 ListVariable("變數名稱")、ListAllVariables()、 ListFunction("名稱")、ListAllFunctions() 等4個額外輸入

6.輸入錯誤時會印出對應的錯誤行數

7. Error可以分出是lexical error(不接受的token @#$等等)、syntactic error(文法錯誤)、 semantic error(呼叫未宣告變數) 

8.輸出部分除了第5點所提及的功能及error訊息外 皆在Done(); 之後輸出
因為做法事 採用先檢查一遍文法正確 將正確文法存下後 第二遍才跑運算的部分
而不是一邊檢查文法一邊運算

9. 變數宣告可以判別出是否有宣告過

# 結果圖
![圖](https://user-images.githubusercontent.com/95120819/192146408-e86b9613-5523-46e1-815f-b158ebcf956b.png)

# 備註
第二次最後一題未通過，可能原因為ListFuction的格式問題
