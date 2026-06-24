# 類 C 語言文法檢查直譯器（Our-C Interpreter）

> 用 C++ 從零實作的「類 C 語言」上下文無關文法檢查器暨直譯器，能判斷輸入是否符合文法，並儲存變數/函式的名稱、內容與型別。中原大學 資訊工程 程式語言（Programming Languages, PL）課堂 Project 2。

![language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![build](https://img.shields.io/badge/build-g%2B%2B%20%2F%20VS%20Code-informational)
![license](https://img.shields.io/badge/License-MIT-green)

---

## 問題（這個程式做什麼）

這是一個迷你版的「Our-C」語言直譯器。它從標準輸入讀進一段類似 C 語言的原始碼，逐行做以下工作：

1. **詞法 / 語法 / 語意檢查**：判斷輸入是否符合所定義的文法。
2. **符號表建立**：把通過檢查的變數、陣列、函式定義（名稱、型別、內容 token）儲存起來。
3. **查詢與執行**：支援列出已定義的變數 / 函式，並在輸入結束後輸出執行結果。

簡單說，它同時扮演「文法檢查器」與「直譯器」兩種角色——先確認文法正確，再執行。

---

## 方法與技術

這是一道偏底層的系統程式 / 編譯原理題目，核心都是手刻的，沒有用任何 parser generator：

- **語言**：C++（單一原始檔 `hw2.cpp`，約 76 KB），開發環境為 Visual Studio Code。
- **手寫詞法分析器（Lexer）**：自行實作 `GetToken()` / `PeekToken()`，直接在 `std::cin` 上逐字 peek / get，並透過 `putback` 做前瞻。能辨識識別字（IDENT）、數字（NUM，含小數點）、字元與字串常值，以及複合運算子，例如 `>=`、`>>`、`<<`、`==`、`!=`、`&&`、`||`、`++`、`+=` 等。
- **遞迴下降式語法分析（Recursive-Descent Parser）**：以一連串互相呼叫的函式處理變數宣告、陣列宣告、函式宣告、運算式、`if` / `while` / `do-while` 條件與迴圈、`cin` / `cout` 輸入輸出等文法結構。
- **以例外（Exception）做錯誤回報**：自訂 `LexicalErrorException`、`SyntaxErrorException`、`UndefinedErrorException` 三種例外類別，外層 `Analyze()` 以 `try / catch` 接住，丟出錯誤後會把該行剩餘字元吃掉再繼續，並以全域變數 `gLine` 追蹤行號。錯誤分為三層：
  - **Lexical error** — 出現不接受的 token（例如 `@`、`#`、`$` 等開頭字元）。
  - **Syntactic error** — 文法錯誤、出現非預期的 token。
  - **Semantic error** — 例如呼叫 / 使用未宣告的識別字。
- **符號表資料結構**：以 `struct Information`（含名稱、型別、`isVariable` / `isFunction` 旗標、陣列值 `vector<float>`、token 串列 `vector<string>`）組成 `vector<Information>` 作為符號表。
- **兩階段（two-pass）處理**：先掃描檢查整段文法是否正確並把定義存下，待讀到 `Done();` 後，第二階段才輸出執行結果，而非一邊檢查一邊運算。

支援的型別 / 關鍵字（從原始碼驗證）：`int`、`void`、`float`、`char`、`bool`、`string`、`if`、`else`、`do`、`while`、`return`、`cin`、`cout`。

內建查詢指令：

| 指令 | 作用 |
| --- | --- |
| `ListVariable("名稱") ;` | 列出指定變數的型別與宣告 |
| `ListAllVariables() ;` | 列出所有已宣告變數 |
| `ListFunction("名稱") ;` | 列出指定函式的完整內容 |
| `ListAllFunctions() ;` | 列出所有已宣告函式名稱 |
| `Done() ;` | 結束輸入，開始輸出執行結果並離開 |

---

## 結果

程式啟動後印出 `Our-C running ...`，逐筆處理輸入，最後印出 `Our-C exited ...`。

### Sample 輸入（節錄自 `input.txt`）

```c
int test ;
char test ;          // re-define 'test'
int salary[30] ;

void Sort(int intArray[30]) {
  int i;
  i=0;
  while(i<29){
    int j;
    j=i;
    while(j<30){
      if(intArray[j]<intArray[i]){
        int temp;
        temp=intArray[i];
        intArray[i]=intArray[j];
        intArray[j]=temp;
      }
      j++ ;
    }
    i++ ;
  }
}

ListAllFunctions() ;
ListFunction( "Sort" ) ;
Done()
```

### 對應輸出（節錄自 `output.txt`）

```
Our-C running ...
> Definition of test entered ...
> New definition of test entered ...
> Definition of salary entered ...
> Definition of Sort() entered ...
> InputSalary()
OutputSalary()
Sort()
Statement executed ...
> void Sort( int intArray[ 30 ] ) {
  int i ;
  i = 0 ;
  while ( i < 29 ) {
    ...
  }
}
Statement executed ...
> Our-C exited ...
```

可以看到：重複定義會印出 `New definition ...`、查詢指令會把符號表內容重新格式化（自動補上空白與縮排）後印出，文法錯誤則會標出對應行號。

完整可重現的輸入 / 輸出範例請見 repo 內的 `input.txt` 與 `output.txt`。

---

## 如何 build 與執行

本專案是**單一 C++ 原始檔**，沒有附帶 Makefile 或專案設定檔；原開發環境為 Visual Studio Code。任何標準 C++ 編譯器皆可編譯。

### 用 g++（命令列）

```bash
# 編譯
g++ hw2.cpp -o our-c

# 執行（程式從 stdin 讀輸入，把結果輸出到 stdout）
./our-c < input.txt
```

> 注意：程式一開始會先讀掉一個字元（一個測試編號）與一個換行，之後才正式開始解析。因此手動互動執行時，**第一行請先隨意輸入一個數字並按 Enter** 才會開始；若改用 `< input.txt` 餵檔，請確保輸入第一行帶有對應的起始數字（範例 `input.txt` 第一行即為此用途）。

### 在 Visual Studio Code

以 C/C++ 擴充套件搭配本機 g++ / MSVC 編譯 `hw2.cpp` 後執行即可，輸入同樣由標準輸入提供。

輸入以 `Done();` 作為結束標記。

---

## 已知限制

- **`ListFunction` 格式問題**：課堂第二次測試的最後一題未通過，研判原因為 `ListFunction` 的輸出格式（縮排 / 空白還原）在某些巢狀結構下不完全符合預期。這是目前已知且未修正的缺陷。
- **單檔、教學取向**：所有邏輯集中在 `hw2.cpp` 一個檔案，未做模組化拆分，亦無自動化測試，僅以 `input.txt` / `output.txt` 做人工對照。
- **輸入流程較特殊**：必須先輸入一個起始數字才會開始解析，且輸入需以 `Done();` 結尾，並非一般可直接貼一段 C 程式碼就跑的介面。
- **僅支援「類 C」子集**：只接受上述列出的關鍵字與文法結構，並非完整 C / C++ 編譯器。

---

## 檔案說明

| 檔案 | 說明 |
| --- | --- |
| `hw2.cpp` | 主程式：詞法分析、語法分析、符號表與直譯邏輯全部在此 |
| `input.txt` | 範例輸入 |
| `output.txt` | 對應的預期輸出 |
| `圖.png` | 執行結果截圖 |

---

> 課程：中原大學 程式語言（PL）Project 2 ｜ 主題：類 C 語言文法檢查與直譯器實作
