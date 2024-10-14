#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 1000
#define MAX_PEOPLE 550
#define MAX_NAME_LEN 20

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/

void add_friend(int friends_graph[MAX_PEOPLE][MAX_PEOPLE], char *user1
, char *user2);
void remove_friend(int friends_graph[MAX_PEOPLE][MAX_PEOPLE], char *user1
, char *user2);
void find_dist(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user1
, int user2);
void number_friends(int friends_graph[MAX_PEOPLE][MAX_PEOPLE]
, int user);
void common_friends(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user1
, int user2);
int get_popularity(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user);
void popular(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user);
void suggestions(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user1);
void handle_input_friends(char *input
, int friends_graph[MAX_PEOPLE][MAX_PEOPLE]);
//int count_words(char **words);
#endif // FRIENDS_H
