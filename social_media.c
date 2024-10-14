/**
 * The entrypoint of the homework. Every initialization must be done here
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
 */
void init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

void free_posts(post_t *post)
{
	if (!post)
	{
		return;
	}

	free_posts(post->child);
	free_posts(post->sibling);

	free(post->title);
	free(post);
}

void free_all_posts(post_t *posts, int num_posts)
{
	for (int i = 0; i < num_posts; i++)
	{
		free_posts(posts[i].child);
		free_posts(posts[i].sibling);
		free(posts[i].title);
	}
	free(posts);
}

/**
 * Entrypoint of the program, compiled with different defines for each task
 */
int main(void)
{
	init_users();

	init_tasks();
	char *input = (char *)malloc(MAX_COMMAND_LEN);
	int friends_graph[MAX_PEOPLE][MAX_PEOPLE] = {0};
	post_t *posts = NULL;
	post_info_t *post_info = malloc(sizeof(post_info_t));
	post_info->num_posts = 0;
	post_info->posts_id = 1;
	post_info->size_allocated = 1;
	while (1)
	{
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);
		char aux1[MAX_COMMAND_LEN];
		strcpy(aux1, input);
		char aux2[MAX_COMMAND_LEN];
		strcpy(aux2, input);
		// If fgets returns null, we reached EOF
		if (!command)
			break;
#ifdef TASK_1
		handle_input_friends(input, friends_graph);
		(void)posts;
#endif

#ifdef TASK_2
		handle_input_posts(aux1, &posts, post_info);
		(void)friends_graph;
#endif

#ifdef TASK_3
		handle_input_feed(aux2, &posts, post_info, friends_graph);
		(void)posts;
		(void)friends_graph;
#endif
	(void)aux1;
	}
	free_all_posts(posts, post_info->num_posts);
	free(post_info);
	free(input);
	free_users();
	return 0;
}
