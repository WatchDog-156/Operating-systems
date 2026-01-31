# PROCESY2 - Code Review
Student: Jakub Borsukiewicz (278319)
Date: 2025-12-01

## Task Completion
[✗] Does the solution meet all assignment requirements?
- Allocates 1,000,000 floats on heap: YES
- Fills array with random values [-1, 1]: YES
- Creates 10 child processes: YES
- Each child calculates sum of 100K elements: YES
- Children write results to "sum<PID>.txt": YES
- Parent waits for all children: YES
- Parent calculates average: NO (incorrect formula)
- Cleanup of temporary files: YES

## Correctness Analysis

### Fork Usage
**Excellent**: Proper fork() implementation with error handling.
**Excellent**: Stores child PIDs in array for later use.
**Excellent**: Child process logic correctly terminates with exit(0).
**Good**: Clean separation of parent and child code paths.

### Memory Management
**Excellent**: Heap allocation using malloc() as required.
**Excellent**: NULL check after malloc() prevents crashes.
**Excellent**: Array is freed at the end of main().
**Good**: Child processes also free memory before exit (good practice even though not strictly necessary).

### File I/O
**Excellent**: Uses fopen/fprintf/fclose correctly.
**Excellent**: NULL check for fopen() failures in both child and parent.
**Excellent**: Checks fscanf() return value (==1) to verify successful read.
**Excellent**: Files are properly closed.
**Excellent**: Uses remove() to clean up temporary files with error checking.

### Array Indexing
**Perfect**: Clean and correct indexing:
```c
for(int el=0; el<100000; el++)
    local_sum+=tablica[el+100000*i];
```
All 1,000,000 elements are correctly processed.

## Error Handling
**Excellent**: malloc() failure handled with informative message.
**Excellent**: fork() failure handled with child number in error message.
**Excellent**: fopen() failures handled for both write and read.
**Excellent**: fscanf() return value checked.
**Excellent**: remove() failure handled with informative message.
**Outstanding**: Comprehensive error handling throughout.

## Resource Management
**Excellent**: All allocated memory is freed (even in error paths).
**Excellent**: All file descriptors are closed.
**Excellent**: Temporary sum*.txt files are removed with proper error checking.

## Code Quality
**Excellent**: Very clean, readable code.
**Good**: Informative printf statements for debugging.
**Good**: Good variable naming.
**Excellent**: Proper use of error messages to inform user.

### Build Configuration
CMakeLists.txt not examined. Should verify compiler warning flags.

### Code Style Issues
**Good**: Clean, consistent code structure.
**Minor**: Could benefit from some symbolic constants (SIZE, NUM_CHILDREN, RANGE).

## Critical Issues
1. **Wrong average calculation**: Line 78 calculates `suma/10.0` instead of `suma/1000000.0`
   - Current: divides sum by 10 (gives sum/10, not average)
   - Should be: `float srednia=suma/1000000.0;`

## Recommendations
1. **CRITICAL FIX**: Line 78 should be `float srednia=suma/1000000.0;` not `suma/10.0`
2. Add symbolic constants for magic numbers:
```c
#define ARRAY_SIZE 1000000
#define NUM_CHILDREN 10
#define RANGE_SIZE (ARRAY_SIZE/NUM_CHILDREN)
```
3. Add compiler warning flags (-Wall, -Wextra) to CMakeLists.txt
4. Consider removing debug printf statements or guarding with #ifdef DEBUG

## Summary
Excellent implementation with outstanding error handling and proper resource management. The code is clean, well-structured, and handles all edge cases properly. However, contains one critical error: calculates sum/10 instead of average (sum/1000000). With this one-line fix, this would be one of the best submissions. Demonstrates strong understanding of system programming concepts.
