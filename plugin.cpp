#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang-tidy/ClangTidyCheck.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/Attr.h"
#include "clang/AST/AttributePlugin.h"

#include "clang/Sema/ParsedAttr.h"
#include "clang/Sema/Sema.h"
#include "clang/Sema/SemaDiagnostic.h"

//#include <type_traits>


namespace clang {
namespace ast_matchers {
namespace {

  template<class AttrType>
  auto hasAttr() {
    return internal::SingleNodeCallbackMatcher<Decl>::Create([](const Decl &N){return N.hasAttr<AttrType>();});
  }

}
}
}


using namespace llvm;
using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;


/*
 *
 * This defines the attribute as attached to AST nodes
 *
 */
struct SignalSafeAttr : public PluginAttribute<SignalSafeAttr> {
  static char ID;

  SignalSafeAttr(ASTContext &Ctx, const AttributeCommonInfo &CommonInfo) : PluginAttribute<SignalSafeAttr>(Ctx, CommonInfo) {}

  const char *getName() const override { return "SignalSafeAttr"; }

  Attr *clone(ASTContext &C) const override { return Create(C, *this); }

  /* These needs proper implementation */
  const char *getSpelling() const override {return "xx";}
  void printPretty(raw_ostream &OS, const PrintingPolicy &Policy) const override {}
};


/*
 *
 * This defines how the attribute should be parsed
 *
 */
struct SignalSafeAttrInfo : public ParsedAttrInfo {
  SignalSafeAttrInfo() {
    static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "signal_safe"}};
    Spellings = S;
  }

  bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr,
                            const Decl *D) const override {

    // Only on function decls!
    if (!isa<FunctionDecl>(D)) {
      S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str)
	<< Attr << Attr.isRegularKeywordAttribute() << "functions";
      return false;
    }
    return true;
  }

  AttrHandling handleDeclAttribute(Sema &S, Decl *D,
                                   const ParsedAttr &Attr) const override {
    D->addAttr(SignalSafeAttr::Create(S.Context, Attr));

    return AttributeApplied;
  }
};
char SignalSafeAttr::ID = 0;

/* this list comes from man signal-safety(7) */
static const auto SIGNAL_SAFE_FUNCTION = hasAnyName("abort",
						    "accept",
						    "access",
						    "aio_error",
						    "aio_return",
						    "aio_suspend",
						    "alarm",
						    "bind",
						    "cfgetispeed",
						    "cfgetospeed",
						    "cfsetispeed",
						    "cfsetospeed",
						    "chdir",
						    "chmod",
						    "chown",
						    "clock_gettime",
						    "close",
						    "connect",
						    "creat",
						    "dup",
						    "dup2",
						    "execl",
						    "execle",
						    "execv",
						    "execve",
						    "_exit",
						    "_Exit",
						    "faccessat",
						    "fchdir",
						    "fchmod",
						    "fchmodat",
						    "fchown",
						    "fchownat",
						    "fcntl",
						    "fdatasync",
						    "fexecve",
						    "ffs",
						    "fork",
						    "fstat",
						    "fstatat",
						    "fsync",
						    "ftruncate",
						    "futimens",
						    "getegid",
						    "geteuid",
						    "getgid",
						    "getgroups",
						    "getpeername",
						    "getpgrp",
						    "getpid",
						    "getppid",
						    "getsockname",
						    "getsockopt",
						    "getuid",
						    "htonl",
						    "htons",
						    "kill",
						    "link",
						    "linkat",
						    "listen",
						    "longjmp",
						    "lseek",
						    "lstat",
						    "memccpy",
						    "memchr",
						    "memcmp",
						    "memcpy",
						    "memmove",
						    "memset",
						    "mkdir",
						    "mkdirat",
						    "mkfifo",
						    "mkfifoat",
						    "mknod",
						    "mknodat",
						    "ntohl",
						    "ntohs",
						    "open",
						    "openat",
						    "pause",
						    "pipe",
						    "poll",
						    "posix_trace_event",
						    "pselect",
						    "pthread_kill",
						    "pthread_self",
						    "pthread_sigmask",
						    "raise",
						    "read",
						    "readlink",
						    "readlinkat",
						    "recv",
						    "recvfrom",
						    "recvmsg",
						    "rename",
						    "renameat",
						    "rmdir",
						    "select",
						    "sem_post",
						    "send",
						    "sendmsg",
						    "sendto",
						    "setgid",
						    "setpgid",
						    "setsid",
						    "setsockopt",
						    "setuid",
						    "shutdown",
						    "sigaction",
						    "sigaddset",
						    "sigdelset",
						    "sigemptyset",
						    "sigfillset",
						    "sigismember",
						    "siglongjmp",
						    "signal",
						    "sigpause",
						    "sigpending",
						    "sigprocmask",
						    "sigqueue",
						    "sigset",
						    "sigsuspend",
						    "sleep",
						    "sockatmark",
						    "socket",
						    "socketpair",
						    "stat",
						    "stpcpy",
						    "stpncpy",
						    "strcat",
						    "strchr",
						    "strcmp",
						    "strcpy",
						    "strcspn",
						    "strlen",
						    "strncat",
						    "strncmp",
						    "strncpy",
						    "strnlen",
						    "strpbrk",
						    "strrchr",
						    "strspn",
						    "strstr",
						    "strtok_r",
						    "symlink",
						    "symlinkat",
						    "tcdrain",
						    "tcflow",
						    "tcflush",
						    "tcgetattr",
						    "tcgetpgrp",
						    "tcsendbreak",
						    "tcsetattr",
						    "tcsetpgrp",
						    "time",
						    "timer_getoverrun",
						    "timer_gettime",
						    "timer_settime",
						    "times",
						    "umask",
						    "uname",
						    "unlink",
						    "unlinkat",
						    "utime",
						    "utimensat",
						    "utimes",
						    "wait",
						    "waitpid",
						    "wcpcpy",
						    "wcpncpy",
						    "wcscat",
						    "wcschr",
						    "wcscmp",
						    "wcscpy",
						    "wcscspn",
						    "wcslen",
						    "wcsncat",
						    "wcsncmp",
						    "wcsncpy",
						    "wcsnlen",
						    "wcspbrk",
						    "wcsrchr",
						    "wcsspn",
						    "wcsstr",
						    "wcstok",
						    "wmemchr",
						    "wmemcmp",
						    "wmemcpy",
						    "wmemmove",
						    "wmemset",
						    "write");

class SignalSafeCheck : public clang::tidy::ClangTidyCheck {
public:
  SignalSafeCheck(clang::StringRef CheckName, clang::tidy::ClangTidyContext *Context)
    : ClangTidyCheck(CheckName, Context) {}

  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override {

    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
				callExpr(callee(functionDecl(unless(anyOf(hasAttr<SignalSafeAttr>(),
									  SIGNAL_SAFE_FUNCTION)))),
					 forCallable(functionDecl(hasAttr<SignalSafeAttr>()).bind("containing-function")))
				.bind("call")),
		       this);

    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
				callExpr(callee(functionDecl(hasName("signal"))),
					 hasArgument(1, declRefExpr(hasDeclaration(functionDecl(unless(hasAttr<SignalSafeAttr>()))
										   .bind("handler")))))
				.bind("setup")),
		       this);

    Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource,
				binaryOperator(hasOperatorName("="),
					       hasLHS(memberExpr(member(hasAnyName("sa_handler", "sa_sigaction")))),
					       hasRHS(declRefExpr(hasDeclaration(functionDecl(unless(hasAttr<SignalSafeAttr>()))
										 .bind("handler")))))
				.bind("setup")),
		       this);

    /* todo: handle designated initializer */

  }

  void check(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const CallExpr *CE = Result.Nodes.getNodeAs<CallExpr>("call")) {
      const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("containing-function");
      diag(CE->getBeginLoc(), "Call to function without signal_safe attribute from signal safe function %0") << FD;
    } else if (const Expr *E = Result.Nodes.getNodeAs<Expr>("setup")) {
      const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("handler");
      diag(E->getBeginLoc(), "Signal handler %0 is missing signal_safe attribute") << FD;
    }
  }
};

class SignalSafeModule : public ClangTidyModule {
public:
  void
  addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<SignalSafeCheck>("signal-safety");
  }
};

static ParsedAttrInfoRegistry::Add<SignalSafeAttrInfo> ATTR("signalsafe", "");
static ClangTidyModuleRegistry::Add<SignalSafeModule> THIS_MODULE("signal-safety", "Clang tidy module for checking signal safety.");
