#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"

void view_post(post_t *post, int is_root, int id_to_check, char *title)
{
	if (!post)
	{
		return;
	}

	if (is_root && post->user_id == id_to_check)
		printf("Posted: %s\n", post->title);
	else if (post->id != -1 && post->user_id == id_to_check)
		printf("Reposted: %s\n", title);
	// Recurse for each child and sibling
	view_post(post->child, 0, id_to_check, title);
	view_post(post->sibling, 0, id_to_check, title);
}

void view_profile(post_t **posts, int num_posts, char *name)
{
	for (int i = 0; i < num_posts; i++)
	{
		view_post(&(*posts)[i], 1, get_user_id(name), (*posts)[i].title);
	}
}

post_t *find_friend_repost(post_t *node, int post_id)
{
	if (!node)
	{
		return NULL;
	}

	if (node->user_id == post_id)
	{
		return node;
	}

	post_t *found_node = find_friend_repost(node->child, post_id);
	if (found_node)
	{
		return found_node;
	}

	return find_friend_repost(node->sibling, post_id);
}

void friends_repost(post_t **posts, char **words
, int friends_matrix[MAX_PEOPLE][MAX_PEOPLE])
{
	int user_id = get_user_id(words[1]);
	int post_id = atoi(words[2]);
	for (int i = 0; i < 520; i++)
	{
		if (friends_matrix[user_id][i] != 0)
		{
			if (find_friend_repost(&(*posts)[post_id - 1], i))
			{
				printf("%s\n", get_user_name(i));
			}
		}
	}
}

void show_feed(post_t **post, int friends_matrix[MAX_PEOPLE][MAX_PEOPLE]
, char **words, post_info_t *post_info)
{
	char *name = words[1];
	int feed_size = atoi(words[2]);
	int user_id = get_user_id(name);
	int count = 0;

	for (int j = post_info->num_posts - 1; j >= 0 && count < feed_size; j--)
	{
		if ((*post)[j].user_id == user_id)
		{
			printf("%s: %s\n", name, (*post)[j].title);
			count++;
		} else {
			for (int k = 0; k < MAX_PEOPLE; k++)
			{
				if (friends_matrix[user_id][k] != 0 && (*post)[j].user_id == k)
				{
					printf("%s: %s\n", get_user_name(k), (*post)[j].title);
					count++;
					break;
				}
			}
		}
	}
}

void handle_input_feed(char *input, post_t **posts, post_info_t *post_info
, int friends_matrix[MAX_PEOPLE][MAX_PEOPLE])
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	int num_words = 0;
	char **words = split_input(input, &num_words);
	if (!cmd)
		return;

	if (!strcmp(cmd, "feed"))
		show_feed(posts, friends_matrix, words, post_info);
	else if (!strcmp(cmd, "view-profile"))
		view_profile(posts, post_info->num_posts, words[1]);
	else if (!strcmp(cmd, "friends-repost"))
		friends_repost(posts, words, friends_matrix);
	else if (!strcmp(cmd, "common-groups"))
		(void)cmd;
	// TODO: Add function
	for (int i = 0; i < num_words; i++)
		free(words[i]);
	free(words);
	free(commands);
	(void)post_info;
	(void)posts;
	(void)friends_matrix;
}
