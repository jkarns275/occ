
unsigned char grayscale[] = {
  7,
  231
  ,255
  ,254
  ,253
  ,188
  ,252
  ,251
  ,250
  ,249
  ,145
  ,248
  ,247
  ,246
  ,245
  ,102
  ,244
  ,243
  ,242
  ,241
  ,59
  ,240
  ,239
  ,238
  ,237
  ,236
  ,235
  ,234
  ,233
  ,232
  ,0
  ,16
};

unsigned char bluescale[] = {
195
,189
,159
,153
,147
,122
,123
,152
,117
,111
,105
,146
,141
,86
,87
,116
,81
,75
,110
,69
,63
,104
,99
,79
,80
,109
,74
,68
,62
,103
,98
,73
,67
,61
,49
,50
,14
,51
,45
,39
,33
,27
,12
,21
,57
,66
,60
,43
,44
,38
,32
,26
,20
,56
,36
,37
,31
,25
,19
,29
,30
,24
,18
,6
,4
,23
,17

};

unsigned char redscale[] = {
53
,54
,90
,89
,91
,127
,126
,125
,128
,164
,163
,162
,161
,96
,165
,13
,201
,200
,199
,198
,197
,133
,132
,170
,169
,168
,139
,171
,207
,206
,205
,204
,176
,175
,213
,212
,211
,182
,219
,218
,225
,5
};

unsigned char yellowscale[] = {
22
,2
,28
,34
,40
,119
,113
,156
,82
,76
,155
,107
,150
,193
,118
,70
,112
,149
,192
,154
,64
,191
,106
,148
,190
,58
,3
,100
,142
,184
,11
,226
,227
,185
,228
,143
,186
,229
,101
,144
,187
,230
,220
,178
,136
,221
,94
,214
,179
,222
,172
,130
,208
,215
,137
,180
,223
,166
,202
,173
,216
,209
};

unsigned char rainbow[] = {
209
,173
,216
,202
,166
,137
,180
,215
,223
,208
,130
,172
,179
,222
,214
,94
,221
,136
,178
,220
,58
,3
,100
,142
,184
,101
,11
,226
,143
,144
,185
,186
,227
,187
,228
,229
,230
,190
,148
,106
,191
,64
,154
,149
,192
,112
,70
,118
,107
,150
,155
,193
,76
,82
,113
,156
,119
,22
,2
,28
,34
,40
,65
,10
,46
,71
,108
,77
,114
,83
,151
,120
,157
,194
,84
,78
,121
,47
,41
,72
,115
,85
,158
,48
,35
,42
,79
,122
,49
,29
,86
,36
,43
,50
,23
,6
,30
,37
,44
,66
,14
,51
,73
,109
,80
,116
,87
,152
,123
,159
,195
,45
,38
,31
,81
,24
,39
,74
,117
,32
,25
,33
,67
,110
,75
,153
,26
,27
,68
,111
,69
,17
,4
,18
,19
,20
,60
,12
,21
,61
,103
,62
,104
,63
,146
,105
,147
,189
,99
,98
,141
,57
,56
,97
,140
,135
,183
,93
,55
,92
,134
,177
,129
,54
,171
,91
,128
,165
,53
,5
,90
,127
,164
,96
,13
,201
,133
,139
,170
,176
,207
,182
,213
,219
,225
,200
,163
,126
,206
,89
,199
,169
,212
,162
,125
,198
,132
,175
,205
,218
,161
,197
,168
,211
,204

};
unsigned char candy[] = {
204
 ,168
 ,211
 ,197
 ,161
 ,132
 ,205
 ,175
 ,218
 ,198
 ,125
 ,162
 ,169
 ,212
 ,199
 ,89
 ,206
 ,126
 ,163
 ,200
 ,53
 ,5
 ,90
 ,127
 ,164
 ,96
 ,13
 ,201
 ,133
 ,170
 ,139
 ,207
 ,176
 ,213
 ,182
 ,219
 ,225
 ,165
 ,128
 ,91
 ,171
 ,54
 ,129
 ,134
 ,177
 ,92
 ,55
 ,93
 ,97
 ,135
 ,140
 ,183
 ,56
 ,57
 ,98
 ,141
 ,99
 ,17
 ,4
 ,18
 ,19
 ,20
 ,60
 ,12
 ,21
 ,61
 ,62
 ,103
 ,63
 ,104
 ,105
 ,146
 ,147
 ,189
 ,69
 ,68
 ,111
 ,27
 ,26
 ,67
 ,75
 ,110
 ,153
 ,33
 ,25
 ,32
 ,74
 ,117
 ,39
 ,24
 ,81
 ,31
 ,38
 ,45
 ,66
 ,14
 ,51
 ,73
 ,80
 ,109
 ,87
 ,116
 ,123
 ,152
 ,159
 ,195
};
#include <cstdio>
#include <vector>
#include <string>
#include <utility>
#include <fmt/core.h>
void ch2str(unsigned char c, std::string &s) {
  switch (c) {
    case '"': s += "\\\""; break;
    case '\\': s += "\\\\"; break;
    case '\a': s += "\\a"; break;
    case '\b': s += "\\b"; break;
    case '\f': s += "\\f"; break;
    case '\n': s += "\\n"; break;
    case '\r': s += "\\r"; break;
    case '\t': s += "\\t"; break;
    case '\v': s += "\\v"; break;
    default:
      if (isprint(c))
        s.push_back((char) c);
      else
        s += fmt::format("\\{:03o}", c);
  }
}

std::string shortest_repr(int len, unsigned char *c) {
  std::string shortest_repr;
  for (int i = 0; i < 256; i++) {
    std::string s;
    ch2str(i, s);
    unsigned char le = (unsigned char) i;
    ch2str((le + (unsigned char) i) & 0xFF, s);
    for (int j = 0; j < len; j++)
      ch2str(0xFF & (c[j] + (unsigned char) i), s);
    if (s.size() < shortest_repr.size() || shortest_repr.size() == 0){
      shortest_repr = s;
    }
  }
  return shortest_repr;
}

int main() {
  std::vector<std::pair<int, unsigned char *>> pal;
  pal.emplace_back(sizeof(grayscale), grayscale);
  pal.emplace_back(sizeof(candy), candy);
  pal.emplace_back(sizeof(bluescale), bluescale);
  pal.emplace_back(sizeof(redscale), redscale);
  pal.emplace_back(sizeof(yellowscale), yellowscale);


  fmt::print("char op[][] = {{");
  for (auto &a : pal)
    fmt::print("\"{}\",\n", shortest_repr(a.first, a.second));
  fmt::print("}};");
}

