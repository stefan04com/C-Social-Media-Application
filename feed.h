#ifndef FEED_H
#define FEED_H
#include "friends.h"
#include "posts.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
 */
void handle_input_feed(char *input, post_t **posts, post_info_t *post_info
, int friends_matrix[MAX_PEOPLE][MAX_PEOPLE]);

void view_post(post_t *post, int is_root, int id_to_check, char *title);

void view_profile(post_t **posts, int num_posts, char *name);

post_t *find_friend_repost(post_t *node, int post_id);

void friends_repost(post_t **posts, char **words
, int friends_matrix[MAX_PEOPLE][MAX_PEOPLE]);

#endif // FEED_H
