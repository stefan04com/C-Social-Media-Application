#ifndef POSTS_H
#define POSTS_H
#define MAX_COMM_PARAM 400
#define MAX_COMM_NUMBER 100

typedef struct post_graph_node post_graph_node;
typedef struct post_t post_t;

typedef struct post_info_t
{
	int num_posts;
	int posts_id;
	int size_allocated;
} post_info_t;

typedef struct like_t
{
	int count;
	int users_likes[520];
} like_t;

typedef struct post_t
{
	int id;
	char *title;
	int user_id;
	like_t likes; // Added the 'likes' field here
	post_t *sibling;
	post_t *child;
} post_t;

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
 */

char **split_input(char *input, int *num_words);

void handle_input_posts(char *input, post_t **posts, post_info_t *post_info);

void create_posts(post_t **posts, post_info_t **post_info, char **words
, int num_words);

post_t *find_post(post_t *node, int post_id);

void repost(char **words, int num_words, post_t **posts
, post_info_t **post_info);

post_t *find_LCA(post_t *root, int id1, int id2);

void common_repost(char **words, post_t **posts, post_info_t **post_info);

void add_like(post_t **posts, char **words, int num_words
, post_info_t **post_info);

void delete_node(post_t *node);

void delete_post(post_t **posts, char **words, int num_words
, post_info_t **post_info);

void get_likes(post_t **posts, char **words, int num_words
, post_info_t **post_info);

post_t *find_most_liked_repost(post_t *post);

void ratio(post_t **posts, char **words);

void print_tree(post_t *post, int is_root);

void print_whole_tree(post_t *post, int is_root);

void get_reposts(post_t **posts, char **words, int num_words
, post_info_t **post_info);

#endif // POSTS_H
