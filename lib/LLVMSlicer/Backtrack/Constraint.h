#ifndef LLVM_CONSTRAINT_H
#define LLVM_CONSTRAINT_H


#include <string>
#include <vector>
#include "Path.h"

namespace llvm {
    namespace slicing {

        class Rule;

        class Parameter {
        public:
            enum ParameterType{
                PRE,
                POST
            };
            Parameter(std::string functionName, uint64_t regNo, ParameterType parameterType);

            std::string getFunctionName() const;
            uint64_t getRegNo() const;
            ParameterType getParameterType() const;

        private:
            std::string functionName;
            uint64_t regNo;
            ParameterType parameterType;
        };

        class Message {
        public:
            Message(Path *path, bool isPrecondition = false);

            void dump();
        private:
            bool isPrecondition;
            Path *path;
        };

        class Constraint {
        public:
            typedef std::vector<const Constraint*> ConstraintList_t;
            enum ConstraintType {
                STRICT,
                WARN,
                PRECONDITION,
                UNSPECIFIED
            };

            enum Type {
                CONSTRAINT,
                RULE
            };

            Constraint(ConstraintType constraintType);

            virtual ~Constraint() {};

            virtual void addConstraint(const Constraint *constraint);

            virtual bool checkConstraint(PathElementBase *pathElement) const = 0;
            virtual bool shouldStop(PathElementBase *pathElement) const = 0;

            virtual Type getType() const;
            ConstraintType getConstraintType() const;

            const ConstraintList_t &getChildren() const;
        protected:
            ConstraintList_t children;
            ConstraintType constraintType;
        private:
        };

        class ChainConstraint : public Constraint {
        public:
            enum ChainType {
                AND,
                OR,
                NOT_AND
            };

            ChainConstraint(ConstraintType constraintType, ChainType chainType);

            virtual bool checkConstraint(PathElementBase *pathElement) const;
            virtual bool shouldStop(PathElementBase *pathElement) const;

        protected:
            ChainType chainType;
        };


        class Rule : public ChainConstraint {
        public:
            typedef std::vector<const Instruction*> InstructionList_t;

            typedef std::pair<Parameter, std::vector<Rule*>> Criterion_t;
            typedef std::vector<Criterion_t> ParameterList_t;
            typedef std::pair<const Rule*, Parameter> RuleParameter_t;
            typedef std::vector<RuleParameter_t> RuleParameterList_t;
            typedef std::pair<const Rule*, Criterion_t> RuleCriterion_t;
            typedef std::vector<RuleCriterion_t> RuleCriterionList_t;
            typedef std::vector<Path*> PathList_t;
            typedef std::pair<const Instruction*, Rule*> InstructionRule_t;
            typedef std::vector<InstructionRule_t> InstructionRuleList_t;
            typedef std::pair<std::pair<const Instruction*, const Instruction*>, InstructionRuleList_t> InitialInstructionPair_t;
            typedef std::vector<InitialInstructionPair_t> InitialInstructionList_t;

            typedef std::vector<Message> MessageList_t;

            enum Result {
                ERROR,
                PRECOND_ERROR,
                VALID
            };

            typedef std::pair<Result, Path*> PathResult_t;
            typedef std::tuple<Result, Rule*, Path*> PrecondResult_t;
            typedef std::vector<PathResult_t> PathResultList_t;
            typedef std::vector<PrecondResult_t> PrecondResultList_t;
            typedef std::pair<PathResult_t, PrecondResultList_t> CompletePathResult_t;
            typedef std::vector<CompletePathResult_t> CompletePathResultList_t;

            Rule(std::string ruleTitle, ConstraintType constraintType, ChainType chainType);
            Rule(const Rule &base, const llvm::Value *criterion);

            void addInitialInstruction(const Instruction *callInst, const Instruction *paramInst, InstructionRuleList_t preconditionInstruction);
            const InitialInstructionList_t &getInitialInstruction() const;

            virtual bool checkConstraint(PathElementBase *pathElement) const;
            virtual bool shouldStop(PathElementBase *pathElement) const;
            virtual bool dismissPath(PathElementBase *pathElementBase) const;

            void addCriterion(Parameter parameter, std::vector<Rule*> preconditions);
            RuleCriterionList_t getCriterions() const;

            virtual Type getType() const;

            virtual void addConstraint(const Constraint *constraint);

            bool preconditions() const;
            bool checkRule();
            bool checkRule(Path *path) const;

            void addPaths(PathList_t paths);
            std::string getRuleTitle() const;

            const CompletePathResultList_t &getResults() const {
                return pathResults;
            };

            const PathList_t &getPaths() const {
                return paths;
            }

            std::vector<const llvm::Value*> getRelevantVariables() const {
                return relevantVariables;
            }

            const llvm::Value *getRelevantLocation() const {
                return relevantLocation;
            }

            bool operator == (const Rule &other) const ;

            void setParentRuleTitle(std::string parent) {
                parentRuleTitle = parent;
            }

            std::string getParentRuleTitle() const {
                return parentRuleTitle;
            }

            void setParentRule(Rule *r) {
                parentRule = r;
            }

            bool useParentPath(Path *path);

            void setDismissable(Path *path) {
                dismissablePaths.insert(path);
            }

            void removeDismissablePaths() {
                for (auto &d : dismissablePaths) {
                    while (std::find(paths.begin(), paths.end(), d) != paths.end()) {
                        paths.erase(std::find(paths.begin(), paths.end(), d));
                    }
                }
            }
        private:
            std::string ruleTitle;
            InitialInstructionList_t initialInstructions;
            ParameterList_t criterions;
            PathList_t paths;

            PathList_t getPaths(const Instruction *inst) const;

            bool checkedPaths = false;
            CompletePathResultList_t pathResults;

            std::vector<const llvm::Value*> relevantVariables;
            const llvm::Value *relevantLocation;
            std::string parentRuleTitle;
            Rule *parentRule;
            std::set<Path*> dismissablePaths;
        };


        class ConstConstraint : public Constraint {
        public:
            enum Compare {
                EQUAL,
                GREATER,
                LOREQ,
                LORNEQ,
                ANY
            };
            ConstConstraint(Compare compare, uint64_t value, ConstraintType constraintType);

            virtual bool checkConstraint(PathElementBase *pathElement) const;
            virtual bool shouldStop(PathElementBase *pathElement) const;

        private:
            uint64_t value;
            Compare compare;
        };

        class CallConstraint : public Constraint {
        public:
            CallConstraint(ConstraintType constraintType, std::string functionName);

            virtual bool checkConstraint(PathElementBase *pathElement) const;
            virtual bool shouldStop(PathElementBase *pathElement) const;
        private:
            std::string functionName;
        };

        class HTMLReportPrinter {
        public:
            HTMLReportPrinter(raw_ostream &file_out);
            void addResults(Rule *rule, const  Rule::CompletePathResultList_t &results);
            void close();
        private:
            raw_ostream &file_out;
            void printHeader();
            void printFooter();
            void printPath(Path *path, bool collapsable);
        };
    }
}

#endif //LLVM_CONSTRAINT_H
