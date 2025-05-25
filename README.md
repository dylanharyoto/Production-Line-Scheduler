Project: Steel-making Production Line Scheduler


Description:
The project aims to optimize the production scheduling of a medium-sized steel-making manufacturer, which currently experiences low machine utilization. In this project, we define ”machine” as ”plant.” Three plants with different production capacities are provided, and the main goal is to utilize the plants as much as possible to speed up the production process, thus enabling more orders to be accepted. 


Given three plants, X, Y, and Z, with capacities of 300, 400, and 500 units of production per day, respectively, given n orders, where Pi = (P1, P2, . . . , Pn) and Pi ∈ [0, ∞), and given the range of working dates for the plants, our goal is to maximize the utilization of these plants by allocating the n orders. We apply the ’First-Come-First-Serve’ (FCFS) and ’Shortest-Job-First’ (SJF) algorithms to find the optimal verifiable solution for scheduling. Finally, we developed a novel heuristic algorithm called ”CHAI”. 



How To Compile & Execute:
1. To compile the program, navigate to the directory containing the source code PLS_G3.c.
2. Open a terminal or command prompt window.
3. Type the command cc PLS_G3.c -o PLS and press Enter. Replace PLS with the desired name for the executable file.
4. Once the compilation is successful, execute the program by typing ./PLS and pressing Enter.

Test Case:
=======================================================================
addPERIOD 2024-12-05 2025-03-07
addBATCH test_data_G03.dat
runPLS CHAI | printREPORT > report_03_CHAI.txt
runPLS FCFS | printREPORT > report_01_FCFS.txt
runPLS SJF | printREPORT > report_02_SJF.txt
exitPLS

Commands:
=======================================================================
Input
addPERIOD 2024-06-01 2024-06-30
Format
addPERIOD [Start Date] [End Date]
Usage
To specify the period (start date and end date) for scheduling the production. Date format is year-month-day, i.e. YYYY-MM-DD.
=======================================================================


Input
addORDER P0001 2024-06-10 2000 Product A
Format
addORDER [Order Number] [Due Date] [Quantity] [Product Name]
Usage
To add an order and the details to the scheduler, followed by the Order Number, Due Date, Quantity, and Product Name. In this project, we assume there are 9 products in 3 categories. Product_A, B and C belong to Category_1; Product_D, E and F belong to Category_2; and Product_G, H and I belong to Category_3.
=======================================================================
Input
addBATCH orderBATCH01.dat
Format
addBATCH [Orders in a batch file]
Usage
It is to input multiple orders in one batch file which is a basic text format. That is, there are many lines of addORDER in the file.
=======================================================================
Input
runPLS FCFS | printREPORT > report_01_FCFS.txt
runPLS SJF | printREPORT > report_02_SJF.txt
runPLS CHAI | printREPORT > report_03_CHAI.txt
Format
runPLS [Algorithm] | printREPORT > [Report file name]
Usage
It is to generate a schedule with the specified Algorithm. It executes a scheduling algorithm. Using the vertical bar [|], the schedule is passed to printREPORT command to print a report of that schedule with the analysis details, for example, the utilization. The greater than sign [>] is to export the report to the given file name.
=======================================================================
Input
exitPLS
Usage
It is to terminate the program. Here we are expecting the program is terminated properly. For example, all child processes have been terminated and collected successfully.






Error Handling
1. If an order's due date does not fall within the specified period, it will not be considered for scheduling and will be appended to `invalid.txt`.
2. If the algorithm name is incorrect, a message will be printed, and the user will be prompted to re-input.
3. If the batch file is empty, a message will be printed indicating that the file is empty and the user will be prompted to re-input.
4. If an incorrect command is inputted into the terminal, a message will be printed, and the user will be prompted to re-input.
