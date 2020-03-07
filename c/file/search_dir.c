#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>

#define FILE_IN_SUFFIX ".profcfgbin"
#define FILE_OUT_SUFFIX ".profbin"

#define FIND_FILENAME "vp_prof.cfgbin"

static int get_com_name_pos(const char *str1, const char *str2)
{
	int pos = 0;
	int len1, len2, min;

	if (strcmp(str1, str2) == 0) {
		printf("Two string are the same: %s == %s\n", str1, str2);
		return -1;
	}

	len1 = strlen(str1);
	len2 = strlen(str2);
	min = (len1 < len2) ? len1: len2;

	/* search common postion frontward*/
	while (pos < min) {
		if (str1[pos] != str2[pos]) {
			break;
		}
		pos++;
	}

	/* search '_' backworkd */
	while (pos > 0) {
		if (str1[pos] == '_') {
			break;
		}
		pos--;
	}

	return pos;
}

static void list_path_file(const char *path)
{
	struct dirent *ptr = NULL;
	DIR *dir = NULL;
	uint32_t n1 = 0, n2 = 0;

	printf("====== list_path_file:\n");
	dir = opendir(path);
	if (!dir) {
		perror(path);
		return;
	}
	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
			continue;
		} else if (ptr->d_type == DT_DIR) {
			printf("Folder: %s\n", ptr->d_name);
			n1++;
		} else {
			printf("File: %s (%ld, %d), len: %ld\n", ptr->d_name, ptr->d_off, ptr->d_reclen,
				strlen(ptr->d_name));
			n2++;
		}
	}
	closedir(dir);
	printf("\nFolder num: %d, File num: %d.\n\n", n1, n2);
}

static int get_pattern_file_num(const char *path,
	char *com_name, uint32_t namesize)
{
	struct dirent **namelist;
	char filename[NAME_MAX+1];
	char *start = NULL;
	int name_len = 0;
	int n, valid_num = 0;

	n = scandir(path, &namelist, NULL, alphasort);
	if (n == -1) {
		perror(path);
		return -1;
	}

	while (n--) {
		printf("ls: %s\n", namelist[n]->d_name);
		if (strstr(namelist[n]->d_name, FILE_IN_SUFFIX) != NULL) {
			if (valid_num == 0) {
				strncpy(filename, namelist[n]->d_name, namesize);
			}
			valid_num++;
		}
		free(namelist[n]);
	}
	free(namelist);

	if (valid_num == 1) {
		start = strrchr(filename, '.');
		name_len = start - filename;
		strncpy(com_name, filename, name_len);
	} else if (valid_num > 1) {
		start = strrchr(filename, '_');
		name_len = start - filename;
		strncpy(com_name, filename, name_len);
	}

	return valid_num;
}

static int find_fname_recursive(const char *path, const char *fname)
{
	struct dirent **namelist;
	char subdir[512];
	int n = 0, valid_num = 0, ret = 0;

	n = scandir(path, &namelist, NULL, alphasort);
	if (n == -1) {
		perror(path);
		return -1;
	}

	while (n--) {
		//printf("ls: %s\n", namelist[n]->d_name);
		if (strcmp(namelist[n]->d_name, ".") == 0 || strcmp(namelist[n]->d_name, "..") == 0) {
			continue;
		} else if (strcmp(namelist[n]->d_name, fname) == 0) {
			valid_num++;
			//printf("file: %s\n", namelist[n]->d_name);
		} else if (namelist[n]->d_type == DT_DIR) {
			snprintf(subdir, sizeof(subdir), "%s/%s", path, namelist[n]->d_name);
			ret = find_fname_recursive(subdir, fname);
			valid_num += ret;
			//printf("folder: %s, ret: %d\n", namelist[n]->d_name, ret);
		}
		free(namelist[n]);
	}
	free(namelist);

	return valid_num;
}

int main(int argc, char *argv[])
{
	const char *app = "search_dir.elf";
	const char *path = NULL;
	int valid_n;
	char *str1="abc_10.bin", *str2="abc_1.bin";
	char com_name[NAME_MAX+1];
	int pos = 0;

	if (argc != 2 ) {
		printf("Usage: %s <Folder>\n", app);
		return -1;
	}
	path = argv[1];

	memset(com_name, 0, sizeof(com_name));
	pos = get_com_name_pos(str1, str2);
	if (!pos) {
		printf("get_com_name_pos err\n");
		return -1;
	}
	strncpy(com_name, str1, pos);
	printf("%s %s the common name pos: %d, %s\n\n", str1, str2, pos, com_name);

	list_path_file(path);

	valid_n = get_pattern_file_num(path, com_name, sizeof(com_name));
	if (valid_n > 0) {
		printf("get_pattern_file_num_in_path: %d, %s\n", valid_n, com_name);
	} else {
		printf("Not find valid pattern file\n");
	}

	printf("========== find_fname_recursive:\n");
	valid_n = find_fname_recursive(path, FIND_FILENAME);
	printf("find_fname_recursive num: %d\n", valid_n);

	return 0;
}
