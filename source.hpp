class String {
private:
    char* strdat;
    uint64_t sz;

    uint64_t strlen(const char* s) {
        const char* c = s;
        uint64_t n = 0;
        while (*c != '\0') {
            ++n;
            c++;
            if (n >= (UINT64_MAX - 10)) {
                printf("Never found null terminator.\n");
                break;
            }
        }
        return n;
    }
    void safe_size(long x) {
        sz = (x >= 0 ? x : 0);
    }
public:
    uint64_t npos = UINT64_MAX;

    /* Constructors */
    String(const char* s) {
        uint64_t s_sz = strlen(s);

        strdat = (char*)malloc(sizeof(char) * s_sz);
        memcpy(strdat, s, sizeof(char) * s_sz);
        strdat[s_sz] = '\0';
        sz = s_sz;
    }
    String(String& os) {
        uint64_t s_s = os.size();
        strdat = (char*)malloc(sizeof(char) * s_s);
        memcpy(strdat, os.data(), sizeof(char) * s_s);
        strdat[s_s] = '\0';
        sz = s_s;
    }
    String(String&& os) noexcept {
        uint64_t s_s = os.size();
        strdat = (char*)malloc(sizeof(char) * s_s);
        memcpy(strdat, os.data(), sizeof(char) * s_s);
        strdat[s_s] = '\0';
        sz = s_s;
    }
    /* Destructor */
  /*  ~String() {
        if (strdat != nullptr) {
            free(strdat);
        }
    }*/

   
  
    /* += ops */
    bool operator+=(const char* d) {
        uint64_t s_sz = strlen(d);
        if (!s_sz) { return 0; }
        char* new_strdat = (char*)malloc((sz + s_sz) * sizeof(char));

        if (new_strdat == nullptr) { return 0; }

        memcpy(new_strdat, strdat, sizeof(char) * sz); //Original Data
        memcpy(new_strdat + sz, d, sizeof(char) * s_sz); //New Data
        new_strdat[sz + s_sz] = '\0';
        free(strdat);

        strdat = new_strdat;
        sz += s_sz;
        return 1;
    }
    bool operator+=(String& s) {
        uint64_t s_sz = s.size();
        if (!s_sz) { return 0; }
        char* new_strdat = (char*)malloc((sz + s_sz) * sizeof(char));

        if (new_strdat == nullptr) { return 0; }

        memcpy(new_strdat, strdat, sizeof(char) * sz); //Original Data
        memcpy(new_strdat + sz, s.data(), sizeof(char) * s_sz); //New Data
        new_strdat[sz + s_sz] = '\0';
        free(strdat);

        strdat = new_strdat;
        sz += s_sz;
        return 1;
    }
    bool operator+=(char d) {
        uint64_t s_sz = 1;
        if (!s_sz) { return 0; }
        char* new_strdat = (char*)malloc((sz + s_sz) * sizeof(char));

        if (new_strdat == nullptr) { return 0; }

        memcpy(new_strdat, strdat, sizeof(char) * sz); //Original Data
        new_strdat[sz] = d;
        new_strdat[sz + s_sz] = '\0';
        free(strdat);

        strdat = new_strdat;
        sz += s_sz;
        return 1;
    }
    
    /* Erase ops */
    int erase(char c) { //Chooses speed over minimal size!
        //Find and rewrite all occurences, then realloc
        char* n_temp = (char*)(malloc(sz * sizeof(char))); //Assume there's none, obviously.
        int idx = 0;
        for (int true_idx = 0; true_idx < sz; true_idx++) {
            if (strdat[true_idx] != c) {
                n_temp[idx] = strdat[true_idx];
                idx++;
            }
        }

        free(strdat);
        strdat = (char*)malloc(sizeof(char) * idx); //idx is quasi "new_size" variable
        memcpy(strdat, n_temp, sizeof(char) * idx);
        strdat[idx] = '\0';
        free(n_temp);
        int erased = sz - idx;
        safe_size(idx-1);
        return erased;
    }
    int erase(const char* c) {
        //Erase an entire sequence. Lol.
        char* n_temp = (char*)malloc(sz * sizeof(char));
        uint64_t seq_len = strlen(c);
        if (seq_len > sz) { return 0; }
        char trigger = *c;
        int idx = 0;
        for (int t_idx = 0; t_idx < sz; t_idx++) {
            if (strdat[t_idx] == trigger) {
                if(t_idx + seq_len >= sz){ /* Ignore! Too long! */
                    n_temp[idx] = strdat[t_idx];
                    idx++;
                }
                bool is_seq = 1;
                for (int j = 1; j < seq_len; j++) {
                    if (strdat[t_idx + j] != c[j]) { /* Nope! Not the sequence! */
                        is_seq = 0;
                        n_temp[idx] = strdat[t_idx];
                        idx++;
                        break;
                    }
                }
                if (is_seq) {
                    t_idx += seq_len - 1;
                }
               
            } else{
                n_temp[idx] = strdat[t_idx];
                idx++;
            }
        }

        free(strdat);
        strdat = (char*)malloc(sizeof(char) * idx);
        memcpy(strdat, n_temp, sizeof(char) * idx);
        strdat[idx] = '\0';
        free(n_temp);
        int erased = sz - idx;
        sz = idx;
        return erased;
    }
    int erase(String& c) {
        //Erase an entire sequence. Lol.
        char* n_temp = (char*)malloc(sz * sizeof(char));
        uint64_t seq_len = c.size();
        if (seq_len > sz) { return 0; }
        char trigger = c[0];
        int idx = 0;
        for (int t_idx = 0; t_idx < sz; t_idx++) {
            if (strdat[t_idx] == trigger) {
                if (t_idx + seq_len >= sz) { /* Ignore! Too long! */
                    n_temp[idx] = strdat[t_idx];
                    idx++;
                }
                bool is_seq = 1;
                for (int j = 1; j < seq_len; j++) {
                    if (strdat[t_idx + j] != c[j]) { /* Nope! Not the sequence! */
                        is_seq = 0;
                        n_temp[idx] = strdat[t_idx];
                        idx++;
                        break;
                    }
                }
                if (is_seq) {
                    t_idx += seq_len - 1;
                }

            }
            else {
                n_temp[idx] = strdat[t_idx];
                idx++;
            }
        }

        free(strdat);
        strdat = (char*)malloc(sizeof(char) * idx);
        memcpy(strdat, n_temp, sizeof(char) * idx);
        strdat[idx] = '\0';
        free(n_temp);
        int erased = sz - idx;
        sz = idx;
        return erased;
    }

    /* Find */
    uint64_t find(char c, uint64_t pos = 0) {
        while (pos < sz) {
            if (*(strdat + pos) == c) {
                return pos;
            }
            pos++;
        }
        return npos;
    }
    uint64_t find(const char* c, uint64_t pos = 0) {
        uint64_t len = strlen(c);
        while (pos < sz) {
            if ((sz - pos) < len) { return npos; }
            if (*(strdat + pos) == *c) {
                //First char matches
                for (int oc = 1; oc < len; oc++) { //'other characters' if you care abt var name
                    if (*(strdat + pos + oc) != *(c + oc)) {//not matching
                        break;
                    }

                    if (oc == (len - 1)) { //wouldve broken by now if wasnt correct string.
                        return pos;
                    }
                }
            }
            pos++;
        }
    }

    /* Substr */
    String substr(uint64_t begin, uint64_t end) {
        if (end > sz) { end = sz; }
        uint64_t subsize = end - begin;

        char* data_transfer = (char*)malloc(sizeof(char) * subsize);
        memcpy(data_transfer, strdat + begin, sizeof(char) * subsize);
        data_transfer[subsize] = '\0';

        String sub = data_transfer; //Performs copy op
        free(data_transfer);
        return sub;
    }

    /* Insert ops */

    bool insert(char c, size_t pos) {
        if (pos >= sz) { return 0; }

        char* new_t = (char*)malloc(sizeof(char) * (sz + 1));
        memcpy(new_t, strdat, sizeof(char) * pos);
        new_t[pos] = c;
        memcpy(new_t + pos + 1, strdat + pos, sizeof(char) * (sz - pos));
        new_t[sz + 1] = '\0';
        free(strdat);
        strdat = new_t;
        sz++;
        return 1;
    }
    bool insert(const char* c, size_t pos) {
        if (pos >= sz) { return 0; }
        uint64_t nsz = strlen(c);

        char* new_t = (char*)malloc(sizeof(char) * (sz + nsz));
        memcpy(new_t, strdat, sizeof(char) * pos);
        memcpy(new_t + pos, c, sizeof(char) * nsz);
        memcpy(new_t + pos + nsz, strdat + pos, sizeof(char) * (sz - pos));
        new_t[sz + nsz] = '\0';
        free(strdat);
        strdat = new_t;
        sz++;
        return 1;
    }
    bool insert(String& c, size_t pos) {
        if (pos >= sz) { return 0; }
        uint64_t nsz = c.size();

        char* new_t = (char*)malloc(sizeof(char) * (sz + nsz));
        memcpy(new_t, strdat, sizeof(char) * pos);
        memcpy(new_t + pos, c.data(), sizeof(char) * nsz);
        memcpy(new_t + pos + nsz, strdat + pos, sizeof(char) * (sz - pos));
        new_t[sz + nsz] = '\0';
        free(strdat);
        strdat = new_t;
        sz++;
        return 1;
    }

    char& operator[](uint64_t i) {
        return *(strdat + i);
    }

    uint64_t size() {
        return sz;
    }
    char* data() {
        return strdat;
    }
    bool empty() {
        return !sz;
    }

    void resize(uint64_t nsz, char filler = ' ') {
        if (nsz == sz || sz<0) { return; }
        else if (nsz > sz) {
            char* ntemp = (char*)malloc(sizeof(char) * nsz);
            memcpy(ntemp, strdat, sizeof(char) * sz);
            memset(ntemp + sz, filler, sizeof(char) * (nsz - sz));
            free(strdat);
            strdat = ntemp;
        }
        else {
            char* ntemp = (char*)malloc(sizeof(char) * nsz);
            memcpy(ntemp, strdat, sizeof(char) * nsz);
            free(strdat);
            strdat = ntemp;
        }

        strdat[nsz] = '\0';
        sz = nsz;
    }

    void push_back(char c) { //lol
        *this += c;
    }
    void append(char c) {
        *this += c;
    }
    void append(const char* c) {
        *this += c;
    }

    char* begin() {
        return strdat;
    }
    char* end() {
        return strdat + sz;
    }

    uint64_t capacity() {
        return (UINT64_MAX-10);
    }
    uint64_t maxsize() {
        return (UINT64_MAX);
    }
    
};
