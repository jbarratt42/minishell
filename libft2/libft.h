/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuezeri <chuezeri@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:00:16 by chuezeri          #+#    #+#             */
/*   Updated: 2025/09/29 15:55:37 by chuezeri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

/*
** Returns the string of digits representing the given int.
*/
char				*ft_itoa(int n);

/*
** Converts c to a lowercase character.
** Returns the converted letter, or c if the conversion was not possible.
*/
int					ft_tolower(int c);

/*
** Checks for any printable character including space.
** Returns non zero if the character is printable, zero otherwise.
*/
int					ft_isprint(int c);

/*
** Checks whether c is a 7-bit unsigned char value that fits into
the ASCII character set.
** Returns non zero if the character is printable, zero otherwise.
*/
int					ft_isascii(int c);

/*
** Converts c to a uppercase character.
** Returns the converted letter, or c if the conversion was not possible.
*/
int					ft_toupper(int c);

/*
** Checks for an alphabetic character or a digit.
** Returns non zero if the character is alphanumerical, and zero if not.
*/
int					ft_isalnum(int c);

/*
** Checks for a digit (0 through 9).
** Returns non zero if the character is a digit, and zero if not.
*/
int					ft_isdigit(int c);

/*
** Checks for an alphabetic character
** Returns non zero if the character is alphabetical, and zero if not
*/
int					ft_isalpha(int c);

/*
** Converts a string to an integer.
** Ignores starting spaces and ending characters which are not digits.
** Returns the int form of the number in the string, or 0 if the string is not
** a number.
*/
int					ft_atoi(const char *str);

/*
** Returns a pointer to a new string which is a duplicate of the input string.
** The new string is allocated with malloc(3) and must be freed with free(3).
*/
char				*ft_strdup(const char *s);

/*
** Returns the length of the given string.
*/
size_t				ft_strlen(const char *str);

/*
** Outputs the string representation of an integer
to the given file descriptor.
*/
void				ft_putnbr_fd(int n, int fd);

/*
** Sets n bits to 0.
*/
void				*ft_bzero(void *s, size_t n);

/*
** Outputs the char the 'c' to the given file descriptor.
*/
void				ft_putchar_fd(char c, int fd);

/*
** Outputs the string 's' to the given file descriptor.
*/
void				ft_putstr_fd(char *s, int fd);
char				*strchr(const char *s, int c);

/*
** Outputs the string ’s’ to the given file descriptor
followed by a newline.
*/
void				ft_putendl_fd(char *s, int fd);

/*
** Returns the array of new strings resulting from the split.
NULL if the allocation fails.
*/
char				**ft_split(char const *s, char c);

/*
** Fill the first n bytes of the memory area pointed to by s
with the constant byte c.
** Returns a pointer to the memory area s.
*/
void				*ft_memset(void *s, int c, size_t n);

/*
** Scans the initial n bytes of the memory area pointed to by s
for the first instance of c.
Both c and the bytes of the memory area pointed to by s
are interpreted as unsigned char.
** Returns a pointer to the matching byte or NULL
if the character does not occur in the given memory area.
*/
void				*ft_memchr(const void *s, int c, size_t n);

/*
** Concatenates the two strings s1 (prefix) and s2 (suffix) into a new string.
** Returns a pointer to the new string, or NULL if the allocation fails.
*/
char				*ft_strjoin(char const *s1, char const *s2);

/*
** Copies n bytes from memory area src to memory area dest.
** The memory areas must not overlap.
** Returns a pointer to dest.
*/
void				*ft_memcpy(void *dest, const void *src, size_t n);

/*
** Compares the first n bytes of type const void s1 and s2.
** Returns 0 if they are equal, a value less than 0 if s1 is less than s2,
or a value greater than 0 if s1 is greater than s2.
*/
int					ft_memcmp(const void *s1, const void *s2, size_t n);

/*
** Concatenates the first size bytes of src to dest.
** The resulting string is stored in the memory area dest, which must be
large enough to hold the result.
** Returns a pointer to dest.
*/
size_t				ft_strlcat(char *dest, const char *src, size_t size);

/*
** Copies the first n characters of src to dst.
** Returns the number of characters copied.
*/
size_t				ft_strlcpy(char *dest, const char *src, size_t size);

/*
** Copies the first n characters of src to dst.
** Returns the string copied.
*/
// char				*ft_strcpy(char *dest, char *src);

/*
** Compares the first n characters of s1 and s2.
** Returns 0 if they are equal, a value less than 0 if s1 is less than s2,
or a value greater than 0 if s1 is greater than s2.
*/
int					ft_strncmp(const char *s1, const char *s2, size_t n);

/*
** Applies the function ’f’ on each character of
the string passed as argument, passing its index
as first argument. Each character is passed by
address to ’f’ to be modified if necessary.
*/
void				ft_striteri(char *s, void (*f)(unsigned int, char *));

/*
** Creates a substring from the string s, starting at index start
and containing len characters.
** Returns a pointer to the new string, which must be freed with free(3)
NULL if the allocation fails.
*/
char				*ft_substr(char const *s, unsigned int start, size_t len);

/*
** Applies the function f to each character of the
string s, passing its index as the first argument
and the character itself as the second.
** Returns the new string, which must be freed with free(3) and
NULL if the allocation fails.
*/
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));

/*
** Locates the first occurrence of the null-terminated string
little in the string big,
where not more than n characters are searched.
*/
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t n);

/*
** Copies n bytes from memory area src to memory area dest.
** The memory areas may overlap: copying takes place as though
the bytes in src are first copied into a temporary array
that does not overlap src or dest, and
the bytes are then copied from the temporary array to dest.
** Returns a pointer to dest.
*/
void				*ft_memmove(void *dest, const void *src, size_t n);

/*
** Fills the first n bytes of the memory area pointed to by s
with the constant byte c.
** Returns a pointer to the memory area s.
*/
void				*ft_memset(void *s, int c, size_t n);

/*
** Returns the index of the first occurrence of the character c
in the string s, or NULL if the character is not found.
*/
char				*ft_strchr(const char *s, int c);

/*
** Returns the index of the last occurrence of the character c
in the string s, or NULL if the character is not found.
*/
char				*ft_strrchr(const char *s, int c);

/*
** Allocates (with malloc(3)) and returns a new node.
The member variable ’content’ is initialized with
the value of the parameter ’content’. The variable
’next’ is initialized to NULL.
*/
t_list				*ft_lstnew(void const *content);

/*
** The length of the list
*/
int					ft_lstsize(t_list *lst);

/*
** Returns the last node of the list.
*/
t_list				*ft_lstlast(t_list *lst);

void				*ft_calloc(size_t nmemb, size_t size);
/*
** Iterates the list ’lst’ and applies the function
’f’ on the content of each node.
*/
void				ft_lstiter(t_list *lst, void (*f)(void *));
/*
** lst: The address of a pointer to the last link of
a list.
new: The address of a pointer to the node to be
added to the list.
*/
void				ft_lstadd_back(t_list **lst, t_list *new_);

/*
** lst: The address of a pointer to the first link of
a list.
new: The address of a pointer to the node to be
added to the list.
*/
void				ft_lstadd_front(t_list **lst, t_list *new_);

/*
** Frees the memory of
the node’s content using the function ’del’ given
as a parameter and free the node. The memory of
’next’ is not freed.
*/
void				ft_lstdelone(t_list *lst, void (*del)(void *));

/*
** Iterates the list ’lst’ and applies the function
’f’ on the content of each node. Creates atry_pipe new
list resulting of the successive applications of
the function ’f’. The ’del’ function is used to
delete the content of a node if needed.
*/
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

/*
** Deletes and frees the given node and every
successor of that node, using the function ’del’
and	free(3).
Finally, the pointer to the list is set to
NULL.
*/
void				ft_lstclear(t_list **lst, void (*del)(void *));

/*
** Returns a unique copy of
’s1’ with the characters specified in ’set’ removed
from the beginning and the end of the string.
*/
char				*ft_strtrim(char const *s1, char const *set);
#endif
