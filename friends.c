#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

char **split_into_words(char *input)
{
	char **words = malloc(2 * sizeof(char *));
	int num_words = 0;
	char *token = strtok(input, "' '\n");
	while (token && num_words < 2)
	{
		words[num_words] = malloc(MAX_NAME_LEN * sizeof(char));
		strcpy(words[num_words], token);
		num_words++;
		token = strtok(NULL, "' '\n");
	}
	words = realloc(words, (num_words + 1) * sizeof(char *));
	words[num_words] = NULL; // Add a NULL pointer at the end
	return words;
}

void free_words(char **words)
{
	for (int i = 0; words[i]; i++)
	{
		free(words[i]);
	}
	free(words);
}

void add_friend(int friends_graph[MAX_PEOPLE][MAX_PEOPLE], char *user1
, char *user2)
{
	friends_graph[get_user_id(user1)][get_user_id(user2)] = 1;
	friends_graph[get_user_id(user2)][get_user_id(user1)] = 1;
	printf("Added connection %s - %s\n", user1, user2);
}

void remove_friend(int friends_graph[MAX_PEOPLE][MAX_PEOPLE]
, char *user1, char *user2)
{
	friends_graph[get_user_id(user1)][get_user_id(user2)] = 0;
	friends_graph[get_user_id(user2)][get_user_id(user1)] = 0;
	printf("Removed connection %s - %s\n", user1, user2);
}

void find_dist(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE]
, int user1, int user2)
{
	int queue[MAX_PEOPLE], front = 0, rear = 0;
	int distance[MAX_PEOPLE];
	for (int i = 0; i < MAX_PEOPLE; i++)
		distance[i] = 99999;
	distance[user1] = 0;
	queue[rear++] = user1;
	while (front != rear)
	{
		int node = queue[front++];
		for (int i = 0; i < MAX_PEOPLE; i++)
		{
			if (friends_matrix[node][i] && distance[i] == 99999)
			{
				distance[i] = distance[node] + 1;
				queue[rear++] = i;
			}
		}
	}

	if (distance[user2] == 99999)
		printf("There is no way to get from %s to %s\n"
		, get_user_name(user1), get_user_name(user2));
	else
		printf("The distance between %s - %s is %d\n"
		, get_user_name(user1), get_user_name(user2), distance[user2]);
}

void number_friends(int friends_graph[MAX_PEOPLE][MAX_PEOPLE], int user)
{
	int count = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
		if (friends_graph[user][i] == 1)
			count++;
	printf("%s has %d friends\n", get_user_name(user), count);
}

void common_friends(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE]
, int user1, int user2)
{
	int ok = 0, i;
	for (i = 0; i < MAX_PEOPLE && !ok; i++)
		if (friends_matrix[user1][i] && friends_matrix[user2][i])
			ok = 1;

	if (!ok)
		printf("No common friends for %s and %s\n", get_user_name(user1)
		, get_user_name(user2));
	else
	{
		printf("The common friends between %s and %s are:\n"
		, get_user_name(user1), get_user_name(user2));
		for (i = 0; i < MAX_PEOPLE; i++)
			if (friends_matrix[user1][i] && friends_matrix[user2][i])
				printf("%s\n", get_user_name(i));
	}
}

int get_popularity(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user)
{
	int count = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
		if (friends_matrix[user][i] == 1)
			count++;
	return count;
}

void popular(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user)
{
	int max_popularity = get_popularity(friends_matrix, user), max_user = user;
	for (int i = 0; i < MAX_PEOPLE; i++)
		if (i != user && get_popularity(friends_matrix, i) > max_popularity)
			if (friends_matrix[user][i] == 1)
			{
				max_popularity = get_popularity(friends_matrix, i);
				max_user = i;
			}

	if (max_popularity == get_popularity(friends_matrix, user))
		printf("%s is the most popular\n", get_user_name(user));
	else
		printf("%s is the most popular friend of %s\n", get_user_name(max_user)
		, get_user_name(user));
}

void suggestions(int friends_matrix[MAX_PEOPLE][MAX_PEOPLE], int user1)
{
	int i, j, k = 0;
	int suggested[MAX_PEOPLE] = {0};
	int suggestions[MAX_PEOPLE];
	int ok = 0;

	for (i = 0; i < MAX_PEOPLE; i++)
		if (friends_matrix[user1][i])
			for (j = 0; j < MAX_PEOPLE; j++)
				if (friends_matrix[i][j] && !friends_matrix[user1][j] &&
				!suggested[j])
					if (j != user1)
					{
						suggestions[k++] = j;
						suggested[j] = 1;
						ok = 1;
					}
	for (i = 0; i < k; i++)
		for (j = i + 1; j < k; j++)
			if (suggestions[i] > suggestions[j])
			{
				int temp = suggestions[i];
				suggestions[i] = suggestions[j];
				suggestions[j] = temp;
			}
	if (ok)
	{
		printf("Suggestions for %s:\n", get_user_name(user1));
		for (i = 0; i < k; i++)
		{
			printf("%s\n", get_user_name(suggestions[i]));
		}
	} else {
		printf("There are no suggestions for %s\n", get_user_name(user1));
	}
}

void handle_input_friends(char *input
, int friends_graph[MAX_PEOPLE][MAX_PEOPLE])
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	char **users = split_into_words(input + strlen(cmd) + 1);
	if (!cmd)
		return;
	if (!strcmp(cmd, "add"))
		add_friend(friends_graph, users[0], users[1]);
	else if (!strcmp(cmd, "remove"))
		remove_friend(friends_graph, users[0], users[1]);
	else if (!strcmp(cmd, "suggestions"))
		suggestions(friends_graph, get_user_id(users[0]));
	else if (!strcmp(cmd, "distance"))
		find_dist(friends_graph, get_user_id(users[0]), get_user_id(users[1]));
	else if (!strcmp(cmd, "common"))
		common_friends(friends_graph, get_user_id(users[0])
		, get_user_id(users[1]));
	else if (!strcmp(cmd, "friends"))
		number_friends(friends_graph, get_user_id(users[0]));
	else if (!strcmp(cmd, "popular"))
		popular(friends_graph, get_user_id(users[0]));
	free_words(users);
	free(commands);
}
