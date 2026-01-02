#pragma once

/**
 * Scans the current directory for existing test case files (e.g., 1.in, 2.in)
 * and returns the next available integer case number to avoid overwriting.
 * * @return The next available case number (int).
 */
int getNextCaseNumber();

/**
 * Runs the interactive console loop for the Forge Test Creator.
 * Allows the user to manually type input and expected output,
 * saving them to numbered .in and .out files automatically.
 * * @return 0 on success, non-zero on error.
 */
int handle_input_tests(bool copy_paste);
