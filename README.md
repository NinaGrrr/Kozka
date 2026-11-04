# Kozka
*A lightweight, optimized Korean vocabulary learning app with strict backend/UI separation.*

Kozka is a personal passion project born out of a simple need: I wanted a foreign language learning app tailored exactly to my own preferences. At the same time, it serves as a practical, hands-on exercise in the complete, end-to-end process of designing, building, and deploying a cross-platform application.

## 🚀 Key Features
* **Optimized Randomization (O(1) Time Complexity):** Uses an auxiliary index vector to guarantee instant, non-repeating random draws, completely eliminating the bottleneck of searching for remaining unread cards in large databases.
* **Embedded JSON Database:** Vocabulary is loaded directly from a resource compiled into the application binary (Qt Resource System / `.qrc`), ensuring seamless deployment to mobile devices. The JSON format was specifically chosen so that entire vocabulary decks can be easily generated using LLMs, completely eliminating the tedious manual entry of each flashcard required by most similar applications.
* **State Management:** Tracks learning progress. Once the daily pool of cards is exhausted, it hides the controls and displays a completion screen.
* **Cross-Platform:** A single codebase compiles natively for Windows (`.exe`) and Android (`.apk`).

## 🛠️ Architecture & Technology Stack
The project follows engineering principles by separating the hardware/logic layer from the lightweight presentation layer:
* **Language & Framework:** C++ / Qt 6 (Qt Quick / QML)
* **Backend (C++):** A central `FlashcardManager` class (inheriting from `QObject`) handles file parsing (`QJsonDocument`, `QFile`), dynamic memory allocation (`std::vector`), and software interrupts (signals).
* **Frontend (QML):** A declarative UI decoupled from business logic, communicating with the C++ engine exclusively through encapsulated public registers (`Q_PROPERTY`), signals (refreshing state), and slots (button actions).
* **Build System:** CMake
<!--
## ⚙️ How to Run Locally
1. Clone the repository to your local machine.
2. Open the `CMakeLists.txt` file in **Qt Creator**.
3. Configure the project by selecting the appropriate toolchain (e.g., **MinGW 64-bit** for Windows or **Android SDK/NDK** for smartphones).
4. Click *Run CMake*, then build and run the project (`Ctrl+R`).
-->

## 🗺️ Roadmap
- [x] Flashcard engine (C++) and UI (QML) implementation
- [x] O(1) non-repeating draw algorithm (`QRandomGenerator`)
- [x] Android port
- [ ] **Vocabulary learning in the context of full sentences**
- [ ] **Grammar learning module**
