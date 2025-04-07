import os
import subprocess
import sys


def run_tests(cpp_program, test_dir):
    # Initialize counters
    total_tests = 0
    passed_tests = 0
    failed_tests = 0

    # Recursively find all JSON files in the test directory
    for root, _, files in os.walk(test_dir):
        for file in files:
            if file.endswith(".json.test"):
                json_file_path = os.path.join(root, file)
                print(f"Running test: {json_file_path}")
                total_tests += 1

                # Determine the expected return code based on the file name
                if "fail" in file:
                    expected_return_code = 1
                elif "pass" in file:
                    expected_return_code = 0
                else:
                    print(f"⚠️  Skipping test (unknown type): {json_file_path}")
                    continue

                # Execute the C++ program with the JSON file as an argument
                result = subprocess.run([cpp_program, json_file_path], capture_output=True, text=True)

                # Check the return code
                if result.returncode == expected_return_code:
                    print(f"✅ Test passed: {json_file_path}")
                    passed_tests += 1
                else:
                    print(f"❌ Test failed: {json_file_path}")
                    print(f"Expected return code: {expected_return_code}, Got: {result.returncode}")
                    failed_tests += 1

                # capture and print the last 21 characters of the JSON file
                cpp_output = result.stdout.strip()
                print(f"{cpp_output}\n")

    print("Test Summary:")
    print(f"Total tests: {total_tests}")
    print(f"Passed tests: {passed_tests}")
    print(f"Failed tests: {failed_tests}")


if __name__ == "__main__":
    # Check if the correct number of arguments is provided
    if len(sys.argv) != 3:
        print("Usage: python test_runner.py <path_to_cpp_program> <path_to_test_directory>")
        sys.exit(1)

    # Get the paths from command-line arguments
    cpp_program = sys.argv[1]
    test_dir = sys.argv[2]

    # Run the tests
    run_tests(cpp_program, test_dir)
