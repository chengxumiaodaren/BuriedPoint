#include "report/buried_report.h"

#include <chrono>
#include <filesystem>

#include "boost/asio/deadline_timer.hpp"
#include "boost/asio/io_service.hpp"
#include "context/context.h"
#include "database/database.h"
#include "report/http_report.h"
#include "spdlog/spdlog.h"
#include "src/third_party/spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace buried {

static const char kDbName[] = "buried.db";

class BuriedReportImpl {
 public:
  BuriedReportImpl(std::shared_ptr<spdlog::logger> logger,
                   CommonService common_service, std::string work_path)
      : logger_(std::move(logger)),
        common_service_(std::move(common_service)),
        work_dir_(std::move(work_path)) {
    if (logger_ == nullptr) {
      logger_ = spdlog::stdout_color_mt("buried");
    }
    SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl init success");
    Context::GetGlobalContext().GetReportStrand().post([this]() { Init_(); });
  }

  ~BuriedReportImpl() = default;

  void Start();

  void InsertData(const BuriedData& data);

 private:
  void Init_();

  void ReportCache_();

  void NextCycle_();

  BuriedDb::Data MakeDbData_(const BuriedData& data);

  std::string GenReportData_(const std::vector<BuriedDb::Data>& datas);

  bool ReportData_(const std::string& data);

 private:
  std::shared_ptr<spdlog::logger> logger_;
  std::string work_dir_;
  std::unique_ptr<BuriedDb> db_;
  CommonService common_service_;

  std::unique_ptr<boost::asio::deadline_timer> timer_;

  std::vector<BuriedDb::Data> data_caches_;
};

void BuriedReportImpl::Init_() {
  std::filesystem::path db_path = work_dir_;
  SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl init db path: {}",
                     db_path.string());
  db_path /= kDbName;
  db_ = std::make_unique<BuriedDb>(db_path.string());
}

void BuriedReportImpl::Start() {
  SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl start");

  timer_ = std::make_unique<boost::asio::deadline_timer>(
      Context::GetGlobalContext().GetMainContext(),
      boost::posix_time::seconds(5));

  timer_->async_wait(Context::GetGlobalContext().GetReportStrand().wrap(
      [this](const boost::system::error_code& ec) {
        if (ec) {
          logger_->error("BuriedReportImpl::Start error: {}", ec.message());
          return;
        }
        ReportCache_();
      }));
}

void BuriedReportImpl::InsertData(const BuriedData& data) {
  Context::GetGlobalContext().GetReportStrand().post(
      [this, data]() { db_->InsertData(MakeDbData_(data)); });
}

bool BuriedReportImpl::ReportData_(const std::string& data) {
  HttpReporter reporter(logger_);
  return reporter.Host(common_service_.host)
      .Topic(common_service_.topic)
      .Port(common_service_.port)
      .Body(data)
      .Report();
}

void BuriedReportImpl::ReportCache_() {
  SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl report cache");
  if (data_caches_.empty()) {
    data_caches_ = db_->QueryData(10);
  }

  if (!data_caches_.empty()) {
    std::string report_data = GenReportData_(data_caches_);
    if (ReportData_(report_data)) {
      db_->DeleteDatas(data_caches_);
      data_caches_.clear();
    }
  }

  NextCycle_();
}

std::string BuriedReportImpl::GenReportData_(
    const std::vector<BuriedDb::Data>& datas) {
  nlohmann::json json_datas;
  for (const auto& data : datas) {
    json_datas.push_back(nlohmann::json::parse(data.content));
  }
  std::string ret = json_datas.dump();
  return ret;
}

BuriedDb::Data BuriedReportImpl::MakeDbData_(const BuriedData& data) {
  BuriedDb::Data db_data;
  db_data.id = -1;
  db_data.priority = data.priority;
  db_data.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::system_clock::now().time_since_epoch())
                          .count();
  nlohmann::json json_data;
  json_data["title"] = data.title;
  json_data["data"] = data.data;
  json_data["user_id"] = common_service_.user_id;
  json_data["app_version"] = common_service_.app_version;
  json_data["app_name"] = common_service_.app_name;
  json_data["custom_data"] = common_service_.custom_data;
  json_data["system_version"] = common_service_.system_version;
  json_data["device_name"] = common_service_.device_name;
  json_data["device_model"] = common_service_.device_model;
  json_data["device_id"] = common_service_.device_id;
  json_data["buried_version"] = common_service_.buried_version;
  json_data["lifecycle_id"] = common_service_.lifecycle_id;
  json_data["priority"] = data.priority;
  json_data["timestamp"] = CommonService::GetNowDate();
  json_data["process_time"] = CommonService::GetProcessTime();
  json_data["report_id"] = CommonService::GetRandomId();
  db_data.content = json_data.dump();

  return db_data;
}

void BuriedReportImpl::NextCycle_() {
  SPDLOG_LOGGER_INFO(logger_, "BuriedReportImpl next cycle");
  timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(5));
  timer_->async_wait([this](const boost::system::error_code& ec) {
    if (ec) {
      logger_->error("BuriedReportImpl::NextCycle_ error: {}", ec.message());
      return;
    }
    Context::GetGlobalContext().GetReportStrand().post(
        [this]() { ReportCache_(); });
  });
}

// ========

BuriedReport::BuriedReport(std::shared_ptr<spdlog::logger> logger,
                           CommonService common_service, std::string work_path)
    : impl_(std::make_unique<BuriedReportImpl>(
          std::move(logger), std::move(common_service), std::move(work_path))) {
}

void BuriedReport::Start() { impl_->Start(); }

void BuriedReport::InsertData(const BuriedData& data) {
  impl_->InsertData(data);
}

BuriedReport::~BuriedReport() {}

}  // namespace buried
