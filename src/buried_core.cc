#include "buried_core.h"

#include "common/common_service.h"
#include "context/context.h"
#include "report/buried_report.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "third_party/nlohmann/json.hpp"

void Buried::InitWorkPath_(const std::string& work_dir) {
  std::filesystem::path _work_dir(work_dir);
  if (!std::filesystem::exists(_work_dir)) {
    std::filesystem::create_directories(_work_dir);
  }

  work_path_ = _work_dir / "buried";
  if (!std::filesystem::exists(work_path_)) {
    std::filesystem::create_directories(work_path_);
  }
}

void Buried::InitLogger_() {
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  std::filesystem::path _log_dir = work_path_ / "buried.log";
  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      _log_dir.string(), true);

  logger_ = std::shared_ptr<spdlog::logger>(
      new spdlog::logger("buried_sink", {console_sink, file_sink}));

  // ref: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  logger_->set_pattern("[%c] [%s:%#] [%l] %v");
  logger_->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Buried::Logger() { return logger_; }

Buried::Buried(const std::string& work_dir) {
  buried::Context::GetGlobalContext().Start();
  InitWorkPath_(work_dir);
  InitLogger_();

  SPDLOG_LOGGER_INFO(Logger(), "Buried init success");
}

Buried::~Buried() {}

BuriedResult Buried::Start(const Config& config) {
  buried::CommonService common_service;
  common_service.host = config.host;
  common_service.port = config.port;
  common_service.topic = config.topic;
  common_service.user_id = config.user_id;
  common_service.app_version = config.app_version;
  common_service.app_name = config.app_name;
  common_service.custom_data = nlohmann::json::parse(config.custom_data);

  buried_report_ = std::make_unique<buried::BuriedReport>(
      logger_, std::move(common_service), work_path_.string());
  buried_report_->Start();
  return BuriedResult::kBuriedOk;
}

BuriedResult Buried::Report(std::string title, std::string data,
                            uint32_t priority) {
  buried::BuriedData buried_data;
  buried_data.title = std::move(title);
  buried_data.data = std::move(data);
  buried_data.priority = priority;
  buried_report_->InsertData(buried_data);
  return BuriedResult::kBuriedOk;
}
