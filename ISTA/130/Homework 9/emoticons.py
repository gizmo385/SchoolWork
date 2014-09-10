"""""
Christopher Chapline
Homework 9
ISTA 130
Adam Stevens
"""

def load_twitter_dicts_from_file(filename, emoticons_to_ids, ids_to_emoticons):
    """Loads emoticons and ids from a file"""
    with open(filename) as f:
        for line in f:
            # Splits the file data into a list
            line_data = line.strip().split()

            # Get the data
            emoticon = line_data[0][1:-1]
            user_id = ( line_data[2][1:-1] )

            if emoticon in emoticons_to_ids:
                id_list = emoticons_to_ids[emoticon]
                id_list.append(user_id)
            else:
                id_list = [user_id]
                emoticons_to_ids[emoticon] = id_list

            if user_id in ids_to_emoticons:
                emoticon_list = ids_to_emoticons[user_id]
                emoticon_list.append( emoticon )
            else:
                emoticon_list = [emoticon]
                ids_to_emoticons[user_id] = emoticon_list

def find_most_common( dictionary ):
    """docstring for find_most_common"""
    longest = list(dictionary.keys())[0]
    for key in dictionary:
        if len(dictionary[key]) > len( dictionary[longest] ):
            longest = key
    return longest

def main():
    """Test the twitter methods"""
    # Load the vlaues from the file
    emoticons_to_ids = {}
    ids_to_emoticons = {}
    load_twitter_dicts_from_file( "twitter_emoticons.dat", emoticons_to_ids, ids_to_emoticons )

    # Print out the number of emoticons and user ids
    print( "Emoticons:", len( emoticons_to_ids.keys() ) )
    print( "UserIDs:", len( ids_to_emoticons.keys() ) )

    # Print out the most common emoticons
    for i in range(5):
        most_common = find_most_common( emoticons_to_ids )
        print( most_common, "occurs", len(emoticons_to_ids[most_common]), "times" )
        emoticons_to_ids.pop(most_common)

if __name__ == '__main__':
    main()


