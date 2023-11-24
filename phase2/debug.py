def process_file(input_file):
    # Read the content of the input file
    with open(input_file, 'r') as file:
        lines = file.readlines()

    # Filter lines based on conditions
    filtered_lines = [line for line in lines if '$' not in line or 'NFLX' in line]

    # Write the modified content back to the input file
    with open(input_file, 'w') as file:
        file.writelines(filtered_lines)

if __name__ == "__main__":
    # Specify the input file path
    input_file_path = "./my_input.txt"  # Change this to your actual file path

    try:
        # Process the file
        process_file(input_file_path)
        print(f"File {input_file_path} processed successfully.")
    except FileNotFoundError:
        print(f"Error: File {input_file_path} not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
