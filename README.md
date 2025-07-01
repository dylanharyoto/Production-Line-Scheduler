# Steel-making Production Line Scheduler

## Overview

This project implements a production line scheduler for a medium-sized steel-making manufacturer, aimed at optimizing machine utilization across three plants (X, Y, and Z) with daily production capacities of 300, 400, and 500 units, respectively. The scheduler processes a set of orders, each with a due date, quantity, and product type, to maximize plant utilization and enable the acceptance of more orders. It supports three scheduling algorithms: First-Come-First-Serve (FCFS), Shortest-Job-First (SJF), and a novel heuristic algorithm called CHAI.

The system consists of a single C program (`PLS_G3.c`) that handles:
- **Order Management**: Accepts individual orders or batch files with order details.
- **Scheduling**: Allocates orders to plants using FCFS, SJF, or CHAI algorithms.
- **Reporting**: Generates reports on plant utilization and scheduling outcomes.
- **Error Handling**: Validates inputs and logs invalid orders to `invalid.txt`.

## Features

- **Optimized Scheduling**: Maximizes utilization of three plants with different capacities.
- **Multiple Algorithms**: Supports FCFS, SJF, and CHAI for flexible scheduling.
- **Order Processing**: Handles individual orders or batch inputs with product categories.
- **Error Handling**: Validates due dates and inputs, logging errors to a file.
- **Reporting**: Outputs detailed reports with utilization metrics.
- **Configurable Period**: Specifies operational periods for scheduling.

## Prerequisites

- **C Compiler**: Requires a C compiler (e.g., `gcc` or `cc`) to compile the program.
- **Operating System**: Compatible with Linux, macOS, or Windows with a C compiler installed.
- **Dependencies**: Input files (e.g., batch order files) in plain text format.

## Installation

1. **Install a C Compiler**:
   - Ensure `gcc` or `cc` is installed on your system. For example, on Ubuntu:
     ```bash
     sudo apt-get install gcc
     ```

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/your-repo-name.git
   cd your-repo-name
   ```

3. **Directory Structure**:
   Ensure the following file is in the project directory:
   - `PLS_G3.c`
   - Optional: Batch order files (e.g., `test_data_G03.dat`)

4. **Compile the Program**:
   Navigate to the project directory and compile the source code:
   ```bash
   cc PLS_G3.c -o PLS
   ```
   Replace `PLS` with your desired executable name if different.

## Usage

1. **Run the Program**:
   Execute the compiled program:
   ```bash
   ./PLS
   ```

2. **Input Commands**:
   Use the following commands interactively in the terminal:
   - **Set Scheduling Period**:
     ```bash
     addPERIOD 2024-12-05 2025-03-07
     ```
     Specifies the operational period for scheduling (format: `YYYY-MM-DD`).

   - **Add a Single Order**:
     ```bash
     addORDER P0001 2024-12-10 2000 Product_A
     ```
     Adds an order with an order number, due date, quantity, and product name. Products are categorized as:
     - Category 1: Product_A, Product_B, Product_C
     - Category 2: Product_D, Product_E, Product_F
     - Category 3: Product_G, Product_H, Product_I

   - **Add a Batch of Orders**:
     ```bash
     addBATCH test_data_G03.dat
     ```
     Processes multiple orders from a text file containing `addORDER` commands.

   - **Run Scheduling and Generate Report**:
     ```bash
     runPLS CHAI | printREPORT > report_03_CHAI.txt
     runPLS FCFS | printREPORT > report_01_FCFS.txt
     runPLS SJF | printREPORT > report_02_SJF.txt
     ```
     Executes the specified algorithm (FCFS, SJF, or CHAI) and outputs a report with utilization details to the specified file.

   - **Exit the Program**:
     ```bash
     exitPLS
     ```
     Terminates the program, ensuring all child processes are properly closed.

3. **Test Case Example**:
   Create a batch file (e.g., `test_data_G03.dat`) with order data, then run:
   ```bash
   addPERIOD 2024-12-05 2025-03-07
   addBATCH test_data_G03.dat
   runPLS CHAI | printREPORT > report_03_CHAI.txt
   runPLS FCFS | printREPORT > report_01_FCFS.txt
   runPLS SJF | printREPORT > report_02_SJF.txt
   exitPLS
   ```

## Configuration Details

- **Input Format**:
  - `addPERIOD [Start Date] [End Date]`: Defines the scheduling period (e.g., `2024-12-05 2025-03-07`).
  - `addORDER [Order Number] [Due Date] [Quantity] [Product Name]`: Specifies order details.
  - `addBATCH [Batch File]`: Loads multiple orders from a text file.
  - `runPLS [Algorithm] | printREPORT > [Report File]`: Runs the specified algorithm and saves the report.
  - `exitPLS`: Terminates the program.

- **Plant Capacities**:
  - Plant X: 300 units/day
  - Plant Y: 400 units/day
  - Plant Z: 500 units/day

- **Product Categories**:
  - Category 1: Product_A, Product_B, Product_C
  - Category 2: Product_D, Product_E, Product_F
  - Category 3: Product_G, Product_H, Product_I

## Error Handling

The program includes robust error handling:
- **Invalid Due Dates**: Orders with due dates outside the specified period are logged to `invalid.txt` and excluded from scheduling.
- **Incorrect Algorithm Name**: Prompts the user to re-enter a valid algorithm (FCFS, SJF, or CHAI).
- **Empty Batch File**: Displays a message and prompts for a valid batch file.
- **Invalid Commands**: Notifies the user of incorrect commands and requests re-input.

## Example Workflow

1. Compile and run the program:
   ```bash
   cc PLS_G3.c -o PLS
   ./PLS
   ```

2. Set the scheduling period and add orders:
   ```bash
   addPERIOD 2024-12-05 2025-03-07
   addORDER P0001 2024-12-10 2000 Product_A
   addBATCH test_data_G03.dat
   ```

3. Generate schedules and reports:
   ```bash
   runPLS CHAI | printREPORT > report_03_CHAI.txt
   runPLS FCFS | printREPORT > report_01_FCFS.txt
   runPLS SJF | printREPORT > report_02_SJF.txt
   ```

4. Exit the program:
   ```bash
   exitPLS
   ```

## Contributing

Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/your-feature`).
3. Make your changes and commit (`git commit -m "Add your feature"`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

Please ensure your code follows the project's coding standards and includes appropriate tests.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or issues, please open an issue on GitHub or contact [your email or preferred contact method].

---

*Last updated: July 1, 2025*
