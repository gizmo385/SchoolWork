"""""
Christopher Chapline
Homework 9
ISTA 130
Adam Stevens
"""

def fish_dict_from_file(filename):
    """Reads in fishcatch data from a file"""
    fish_names = { 1 : "Bream", 2 : "Whitefish", 3 : "Roach", 4 : "?",
            5 : "Smelt", 6 : "Pike", 7 : "Perch" }

    # Map fish names to a list of weights
    fish_weights = { fish_names[i]:[] for i in range(1, len(fish_names) + 1 ) }

    with open( filename ) as f:
        for line in f:
            # Split the line into a list
            line_data = line.split()

            # Get list of weights for the fish's name
            fish_id_code = int( line_data[1] )
            fish_name = fish_names[fish_id_code]
            fish_weight_list = fish_weights[ fish_name ]

            # Get the weight from the file data
            fish_weight = line_data[2].strip()

            # If the weight is valid, add it to the list
            if fish_weight != "NA":
                fish_weight_list.append( float( line_data[2] ) )
    return fish_weights


def main():
    """""
    Prints out the information from the "fishcatch.dat" file
    """
    fish_dict = fish_dict_from_file( "fishcatch.dat" )

    print( " # NAME\t\tMEAN WT" )
    for fish in sorted( fish_dict.keys() ):
        # Calculate the mean weight
        fish_weights = fish_dict[fish]
        mean_weight = sum(fish_weights) / len( fish_weights )

        # Print out the information
        number = str(len(fish_weights)).rjust(2, " ")
        print( number, end="" )
        print( " " + fish, end="" )
        mean_string = str(round(mean_weight, 1) ) + "g"
        print( mean_string.rjust( 13 + len(mean_string) - len(fish) ), end="")
        print()

if __name__ == '__main__':
    main()
