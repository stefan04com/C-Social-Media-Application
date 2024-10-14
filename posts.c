#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

char **split_input(char *input, int *num_words)
{
	char **words = malloc(MAX_COMM_NUMBER * sizeof(char *));
	char *token = strtok(input, "\n");
	while (token && (*num_words) < MAX_COMM_NUMBER)
	{
		char *space = strchr(token, ' ');
		while (space)
		{
			*space = '\0'; // Replace the space with a null terminator
			words[*num_words] = malloc((space - token + 1) * sizeof(char));
			strcpy(words[*num_words], token);
			(*num_words)++;
			token = space + 1;
			space = strchr(token, ' ');
		}
		words[*num_words] = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(words[*num_words], token);
		(*num_words)++;
		token = strtok(NULL, "\n");
	}
	words = realloc(words, (*num_words) * sizeof(char *));
	return words;
}

void create_posts(post_t **posts, post_info_t **post_info, char **words
, int num_words)
{
	if ((*post_info)->num_posts == 0)
	{
		(*posts) = (post_t *)malloc(sizeof(post_t));
		(*posts)[0].id = (*post_info)->posts_id;
		(*posts)[0].user_id = get_user_id(words[1]);
		(*posts)[0].title = (char *)malloc(MAX_COMM_PARAM * sizeof(char));
		strcpy((*posts)[0].title, "");
		for (int i = 2; i < num_words; i++)
		{
			strcat((*posts)[0].title, words[i]);
			if (i != num_words - 1)
				strcat((*posts)[0].title, " ");
		}
		(*posts)[0].sibling = NULL;
		(*posts)[0].child = NULL;
		(*posts)[0].likes.count = 0;
		for (int i = 0; i < 520; i++)
			(*posts)[0].likes.users_likes[i] = 0;
		printf("Created %s for %s\n", (*posts)[(*post_info)->num_posts].title
		, get_user_name((*posts)[(*post_info)->num_posts].user_id));
		//(*post_info)->size_allocated = 1;
		// printf("%d\n" , (*posts)->id);
		(*post_info)->num_posts++;
	} else {
		if ((*post_info)->num_posts == (*post_info)->size_allocated)
		{
			(*post_info)->size_allocated *= 2;
			(*posts) = (post_t *)realloc((*posts), (*post_info)->size_allocated
			* sizeof(post_t));
		}
		// printf("%d\n" , (*post_info)->size_allocated);
		// posts[(*post_info)->num_posts] = (post_t *)malloc(sizeof(post_t));
		(*posts)[(*post_info)->num_posts].id = (*post_info)->posts_id;
		(*posts)[(*post_info)->num_posts].user_id = get_user_id(words[1]);
		(*posts)[(*post_info)->num_posts].title = (char *)malloc(MAX_COMM_PARAM
		* sizeof(char));
		strcpy((*posts)[(*post_info)->num_posts].title, "");
		for (int i = 2; i < num_words; i++)
		{
			strcat((*posts)[(*post_info)->num_posts].title, words[i]);
			if (i != num_words - 1)
				strcat((*posts)[(*post_info)->num_posts].title, " ");
		}
		(*posts)[(*post_info)->num_posts].sibling = NULL;
		(*posts)[(*post_info)->num_posts].child = NULL;
		(*posts)[(*post_info)->num_posts].likes.count = 0;
		for (int i = 0; i < 520; i++)
		{
			(*posts)[(*post_info)->num_posts].likes.users_likes[i] = 0;
		}
		printf("Created %s for %s\n", (*posts)[(*post_info)->num_posts].title
		, get_user_name((*posts)[(*post_info)->num_posts].user_id));
		// printf("%d\n" , posts[(*post_info)->num_posts]->id);
		//(*post_info)->posts_id++;
		// printf("%d\n" , (*post_info)->posts_id);
		(*post_info)->num_posts++;
	}
	(*post_info)->posts_id++;
}

post_t *find_post(post_t *node, int post_id)
{
	if (!node)
	{
		return NULL;
	}

	if (node->id == post_id)
	{
		return node;
	}

	post_t *found_node = find_post(node->child, post_id);
	if (found_node)
	{
		return found_node;
	}

	return find_post(node->sibling, post_id);
}

int find_index_repost(post_t **posts, post_info_t **post_info, int post_id)
{
	int id = 0;
	for (int i = 0; i < (*post_info)->num_posts; i++)
	{
		if ((*posts)[i].id == post_id)
		{
			id = i;
			break;
		}
	}
	return id;
}

void repost(char **words, int num_words, post_t **posts
, post_info_t **post_info)
{
	int id = 0;
	if (num_words == 3)
	{
		int post_id = atoi(words[2]);
		id = find_index_repost(posts, post_info, post_id);
		post_id--;
		if (!(*posts)[id].child)
		{
			(*posts)[id].child = (post_t *)malloc(sizeof(post_t));
			(*posts)[id].child->id = (*post_info)->posts_id;
			(*posts)[id].child->title = NULL;
			(*posts)[id].child->user_id = get_user_id(words[1]);
			(*posts)[id].child->sibling = NULL;
			(*posts)[id].child->child = NULL;
			(*posts)[id].child->likes.count = 0;
			for (int i = 0; i < 520; i++)
				(*posts)[id].child->likes.users_likes[i] = 0;
		} else {
			post_t *aux = (*posts)[id].child;
			while (aux->sibling)
			{
				aux = aux->sibling;
			}
			aux->sibling = (post_t *)malloc(sizeof(post_t));
			aux->sibling->id = (*post_info)->posts_id;
			aux->sibling->title = NULL;
			aux->sibling->user_id = get_user_id(words[1]);
			aux->sibling->sibling = NULL;
			aux->sibling->child = NULL;
			aux->sibling->likes.count = 0;
			for (int i = 0; i < 520; i++)
				aux->sibling->likes.users_likes[i] = 0;
		}
	} else if (num_words == 4) {
		int post_id = atoi(words[2]);
		int repost_id = atoi(words[3]);
		id = find_index_repost(posts, post_info, post_id);
		post_t *repost = find_post(&(*posts)[id], repost_id);
		if (!repost)
		{
			printf("repost %d not found\n", repost_id);
			return;
		}
		if (!repost->child)
		{
			repost->child = (post_t *)malloc(sizeof(post_t));
			repost->child->id = (*post_info)->posts_id;
			repost->child->title = NULL;
			repost->child->user_id = get_user_id(words[1]);
			repost->child->sibling = NULL;
			repost->child->child = NULL;
			repost->child->likes.count = 0;
			for (int i = 0; i < 520; i++)
				repost->child->likes.users_likes[i] = 0;
		} else {
			post_t *aux = repost->child;
			while (aux->sibling)
			{
				aux = aux->sibling;
			}
			aux->sibling = (post_t *)malloc(sizeof(post_t));
			aux->sibling->id = (*post_info)->posts_id;
			aux->sibling->title = NULL;
			aux->sibling->user_id = get_user_id(words[1]);
			aux->sibling->sibling = NULL;
			aux->sibling->child = NULL;
			aux->sibling->likes.count = 0;
			for (int i = 0; i < 520; i++)
				aux->sibling->likes.users_likes[i] = 0;
		}
	}
	printf("Created repost #%d for %s\n", (*post_info)->posts_id
	, get_user_name(get_user_id(words[1])));
	(*post_info)->posts_id++;
}

post_t *find_LCA(post_t *root, int id1, int id2)
{
	if (!root)
	{
		return NULL;
	}

	if (root->id == id1 || root->id == id2)
	{
		return root;
	}

	post_t *current_child = root->child;
	post_t *found = NULL;
	while (current_child)
	{
		post_t *result = find_LCA(current_child, id1, id2);
		if (result)
		{
			if (found)
			{
				return root;
			}
			found = result;
		}
		current_child = current_child->sibling;
	}
	return found;
}

void common_repost(char **words, post_t **posts, post_info_t **post_info)
{
	int root_id = atoi(words[1]);
	int post_id1 = atoi(words[2]);
	int post_id2 = atoi(words[3]);
	int id = find_index_repost(posts, post_info, root_id);
	post_t *lca = find_LCA(&(*posts)[id], post_id1, post_id2);
	if (!lca)
	{
		printf("No common reposts for %d and %d\n", post_id1, post_id2);
		return;
	}
	printf("The first common repost of %d and %d is %d\n", post_id1
	, post_id2, lca->id);
}

void add_like(post_t **posts, char **words, int num_words
, post_info_t **post_info)
{
	int id = 0;
	if (num_words == 3)
	{
		int post_id = atoi(words[2]);
		int user_id = get_user_id(words[1]);
		id = find_index_repost(posts, post_info, post_id);
		if ((*posts)[id].likes.users_likes[user_id] == 1)
		{
			printf("User %s unliked post %s\n", get_user_name(user_id)
			, (*posts)[id].title);
			(*posts)[id].likes.count--;
			(*posts)[id].likes.users_likes[user_id] = 0;
			return;
		}
		(*posts)[id].likes.count++;
		(*posts)[id].likes.users_likes[user_id] = 1;
		printf("User %s liked post %s\n", get_user_name(user_id)
		, (*posts)[id].title);
	} else if (num_words == 4) {
		int post_id = atoi(words[2]);
		int user_id = get_user_id(words[1]);
		int repost_id = atoi(words[3]);
		id = find_index_repost(posts, post_info, post_id);
		post_t *repost = find_post(&(*posts)[id], repost_id);
		if (repost->likes.users_likes[user_id] == 1)
		{
			printf("User %s unliked repost %s\n", get_user_name(user_id)
			, (*posts)[id].title);
			repost->likes.count--;
			repost->likes.users_likes[user_id] = 0;
			return;
		}
		repost->likes.count++;
		repost->likes.users_likes[user_id] = 1;
		printf("User %s liked repost %s\n", get_user_name(user_id)
		, (*posts)[id].title);
	}
}

void get_likes(post_t **posts, char **words, int num_words
, post_info_t **post_info)
{
	int id = 0;
	if (num_words == 2)
	{
		int post_id = atoi(words[1]);
		id = find_index_repost(posts, post_info, post_id);
		printf("Post %s has %d likes\n", (*posts)[id].title
		, (*posts)[id].likes.count);
	} else if (num_words == 3) {
		int post_id = atoi(words[1]);
		int repost_id = atoi(words[2]);
		id = find_index_repost(posts, post_info, post_id);
		post_t *repost = find_post(&(*posts)[id], repost_id);
		printf("Repost #%d has %d likes\n", repost_id, repost->likes.count);
	}
}

void delete_node(post_t *node)
{
	if (!node)
	{
		return;
	}

	post_t *current_child = node->child;
	while (current_child)
	{
		post_t *next_child = current_child->sibling;
		delete_node(current_child);
		current_child = next_child;
	}
	node->id = -1;
}

void delete_post(post_t **posts, char **words, int num_words
, post_info_t **post_info)
{
	int post_id = 0;
	int repost_id = 0;
	int id = 0;
	if (num_words == 2)
	{
		post_id = atoi(words[1]);
		id = find_index_repost(posts, post_info, post_id);
		printf("Deleted %s\n", posts[id]->title);
		delete_node(&(*posts)[id]);
		posts[id] = NULL;
	} else if (num_words == 3) {
		post_id = atoi(words[1]);
		repost_id = atoi(words[2]);
		id = find_index_repost(posts, post_info, post_id);
		post_t *post = find_post(&(*posts)[id], repost_id);
		if (post)
		{
			printf("Deleted repost #%d of post %s\n", post->id
			, (*posts)[id].title);
			delete_node(post);
		}
	}
}

post_t *find_most_liked_repost(post_t *post)
{
	if (!post)
	{
		return NULL;
	}
	post_t *most_liked_repost = post;
	if (post->child)
	{
		post_t *child_most_liked = find_most_liked_repost(post->child);
		if (child_most_liked->likes.count > most_liked_repost->likes.count)
		{
			most_liked_repost = child_most_liked;
		}
	}
	if (post->sibling)
	{
		post_t *sibling_most_liked = find_most_liked_repost(post->sibling);
		if (sibling_most_liked->likes.count > most_liked_repost->likes.count)
		{
			most_liked_repost = sibling_most_liked;
		}
	}
	return most_liked_repost;
}

void ratio(post_t **posts, char **words)
{
	int post_id = atoi(words[1]) - 1;
	post_t *most_liked_repost = find_most_liked_repost(&(*posts)[post_id]);
	if (most_liked_repost->likes.count == (*posts)[post_id].likes.count)
	{
		printf("The original post is the highest rated\n");
		return;
	}
	printf("Post %d got ratio'd by repost %d\n", (*posts)[post_id].id
	, most_liked_repost->id);
}

void print_tree(post_t *post, int is_root)
{
	if (!post)
	{
		return;
	}

	if (is_root)
		printf("%s - Post by %s\n", post->title, get_user_name(post->user_id));
	else if (post->id != -1)
		printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
	// Recurse for each child and sibling
	print_tree(post->child, 0);
}

void print_whole_tree(post_t *post, int is_root)
{
	if (!post)
	{
		return;
	}

	if (is_root)
		printf("%s - Post by %s\n", post->title, get_user_name(post->user_id));
	else if (post->id != -1)
		printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
	// Recurse for each child and sibling
	print_whole_tree(post->child, 0);
	print_whole_tree(post->sibling, 0);
}

void get_reposts(post_t **posts, char **words, int num_words
, post_info_t **post_info)
{
	int post_id = 0;
	int repost_id = 0;
	int id = 0;
	if (num_words == 2)
	{
		post_id = atoi(words[1]);
		id = find_index_repost(posts, post_info, post_id);
		print_whole_tree(&(*posts)[id], 1);
	} else if (num_words == 3) {
		post_id = atoi(words[1]);
		repost_id = atoi(words[2]);
		id = find_index_repost(posts, post_info, post_id);
		post_t *post = find_post(&(*posts)[id], repost_id);
		if (post)
			print_tree(post, 0);
	}
}

void handle_input_posts(char *input, post_t **posts, post_info_t *post_info)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	int num_words = 0;
	char **words = split_input(input, &num_words);
	if (!cmd)
		return;

	if (!strcmp(cmd, "create"))
		create_posts(posts, &post_info, words, num_words);
	else if (!strcmp(cmd, "repost"))
		repost(words, num_words, posts, &post_info);
	else if (!strcmp(cmd, "common-repost"))
		common_repost(words, posts, &post_info);
	else if (!strcmp(cmd, "like"))
		add_like(posts, words, num_words, &post_info);
	else if (!strcmp(cmd, "ratio"))
		ratio(posts, words);
	else if (!strcmp(cmd, "delete"))
		delete_post(posts, words, num_words, &post_info);
	else if (!strcmp(cmd, "get-likes"))
		get_likes(posts, words, num_words, &post_info);
	else if (!strcmp(cmd, "get-reposts"))
		get_reposts(posts, words, num_words, &post_info);
	for (int i = 0; i < num_words; i++)
		free(words[i]);
	free(words);
	free(commands);
}
