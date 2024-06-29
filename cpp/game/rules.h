#ifndef GAME_RULES_H_
#define GAME_RULES_H_

#include "../core/global.h"
#include "../core/hash.h"

#include "../external/nlohmann_json/json.hpp"

struct Rules {
  static const int SIXWINRULE_ALWAYS = 0; //six or longer is always win
  static const int SIXWINRULE_NEVER = 1;  // six or longer is never win
  static const int SIXWINRULE_CARO = 2;   // oxxxxxx. is win, oxxxxxxo is not win
  int sixWinRule;

  bool wallBlock;// whether regard wall as opp stone (if true, oxxxxx# is not win)

  static const int VCNRULE_NOVC = 0;
  static const int VCNRULE_VC1_B = 1;
  static const int VCNRULE_VC2_B = 2;
  static const int VCNRULE_VC3_B = 3;
  static const int VCNRULE_VC4_B = 4;
  static const int VCNRULE_VC1_W = 11;
  static const int VCNRULE_VC2_W = 12;
  static const int VCNRULE_VC3_W = 13;
  static const int VCNRULE_VC4_W = 14;
  int VCNRule;

  bool firstPassWin;  // �����ʱ����pass�Ļ�ʤ

  int maxMoves;  // �ﵽ�����ֱ�Ӻ��壬0������


  Rules();
  Rules(int sixWinRule, bool wallBlock, int VCNRule, bool firstPassWin, int maxMoves);
  ~Rules();

  bool operator==(const Rules& other) const;
  bool operator!=(const Rules& other) const;

  static std::set<std::string> SixWinRuleStrings();
  static std::set<std::string> VCNRuleStrings();

  static Rules getTrompTaylorish();

  static int parseSixWinRule(std::string s);
  static std::string writeSixWinRule(int sixWinRule);

  static int parseVCNRule(std::string s);
  static std::string writeVCNRule(int VCNRule);


  static Rules parseRules(const std::string& str);
  static bool tryParseRules(const std::string& str, Rules& buf);

  static Rules updateRules(const std::string& key, const std::string& value, Rules priorRules);

  Color vcSide() const;
  int vcLevel() const;

  friend std::ostream& operator<<(std::ostream& out, const Rules& rules);
  std::string toString() const;
  std::string toStringMaybeNice() const;
  std::string toJsonString() const;
  nlohmann::json toJson() const;
  
  static const Hash128 ZOBRIST_SIXWIN_RULE_HASH[3];
  static const Hash128 ZOBRIST_WALLBLOCK_HASH;
  static const Hash128 ZOBRIST_VCNRULE_HASH_BASE;
  static const Hash128 ZOBRIST_FIRSTPASSWIN_HASH;
  static const Hash128 ZOBRIST_MAXMOVES_HASH_BASE;
  static const Hash128 ZOBRIST_PASSNUM_B_HASH_BASE;
  static const Hash128 ZOBRIST_PASSNUM_W_HASH_BASE;
};

#endif  // GAME_RULES_H_
