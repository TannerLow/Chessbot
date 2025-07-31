import chess
import os

def verify_game(filename) -> bool:
    moves_to_verify = []
    with open(filename, 'r') as file:
        for line in file.readlines():
            if len(line.strip()) != 0:
                moves_to_verify.append(line.strip())

    # A list of moves to validate. The 5th move, 'Qf7', is illegal.
    # White can't move twice in a row.
    #moves_to_verify = ['e4', 'e5', 'Qh5', 'Nc6', 'Qf7'] 

    board = chess.Board() # Creates a board in the standard starting position

    #print(f"Starting Position:\n{board}\n")

    for i, move_san in enumerate(moves_to_verify):
        try:
            # This is the core validation step.
            # It checks if the move is legal and pushes it to the board's move stack.
            board.push_san(move_san)
            #print(f"✅ Move {i+1} ('{move_san}') is legal.")
            #print(f"Board after '{move_san}':\n{board}\n")
        except chess.IllegalMoveError:
            print(f"❌ ERROR: Move {i+1} ('{move_san}') is illegal!")
            print("Verification failed.")
            return False
    else:
        # This block only runs if the loop completes without a 'break'.
        print("🎉 All moves in the sequence are legal!")
        return True


def validate_directory(directory_path):
    print(f"Searching for files in: {os.path.abspath(directory_path)}\n")

    try:
        for filename in os.listdir(directory_path):
            full_path = os.path.join(directory_path, filename)

            if os.path.isfile(full_path):
                print(f"Found file: {filename}")
                if not verify_game(full_path):
                    print("Illegal move detected!:", filename)
                    return

    except FileNotFoundError:
        print(f"Error: Directory not found at '{directory_path}'")

#validate_directory('Chessbot/movesets/')
print(verify_game('Chessbot/movesets/moveset.lan'))
