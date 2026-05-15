# 🧠 CLAUDE VIBE CODING — Quy Trình Tiếp Cận Bài Toán

> File này định nghĩa cách Claude suy nghĩ, phân tích và giải quyết bài toán trong suốt quá trình vibe coding.
> Đưa file này vào IDE hoặc đính kèm vào đầu mỗi session để Claude hoạt động nhất quán.
>
> **Tích hợp:** rule-00 (thinking framework) · rule-01 (data engineering) · rule-02 (ml pipeline) · rule-03 (code structure)

---

## 🎯 NGUYÊN TẮC CỐT LÕI (BẤT BIẾN)

```
EXPLICIT > IMPLICIT     — Viết rõ ràng hơn viết thông minh
SIMPLE > CLEVER         — Code đơn giản dễ debug hơn code khéo léo
CORRECT > FAST          — Đúng trước, tối ưu sau
FAIL LOUD               — Lỗi phải raise exception rõ ràng, không silent fail
LOG EVERYTHING          — Mỗi bước phải log input size, output size, thời gian
CONFIG > HARDCODE       — Mọi thứ có thể thay đổi theo môi trường → config
TEST EARLY              — Viết test ngay khi viết logic, không để sau
```

---

## 📐 QUY TRÌNH TIẾP CẬN BÀI TOÁN

### PHASE 0 — 🔍 CLARIFY TRƯỚC, CODE SAU (Không Bỏ Qua)

Trước khi viết bất kỳ dòng code nào, Claude phải tự trả lời đủ 5 câu hỏi này.
**Nếu câu nào chưa có thông tin → hỏi người dùng, không tự suy diễn.**

```
1. INPUT là gì?
   - Nguồn: file / DB / API / stream / model output?
   - Schema / shape / dtype đã biết chưa?
   - Volume ước tính: hàng nghìn / triệu / tỷ rows?

2. OUTPUT là gì?
   - Dạng: file / table / model artifact / API response?
   - Downstream consumer là ai/cái gì đọc output này?

3. TRIGGER là gì?
   - Chạy thủ công / schedule / event-driven / realtime?

4. FAILURE MODE là gì?
   - Khi lỗi giữa chừng: retry / skip / alert / fail-fast?
   - Idempotent không? Chạy lại có safe không?

5. ENVIRONMENT là gì?
   - Local / Docker / Cloud (GCP / AWS / Azure)?
   - Ngôn ngữ, framework, version cụ thể?
```

> **Rule:** Assumption phải được ghi tường minh bằng comment `# ASSUMPTION: ...` trong code — không để ngầm hiểu.

---

### PHASE 1 — 🗺️ DECOMPOSE TRƯỚC, CODE SAU

Claude KHÔNG viết code ngay. Phải decompose thành các bước độc lập trước.

**Template decomposition:**

```
Pipeline / Feature: [Tên]

BƯỚC 1: [Tên bước] — [Mô tả 1 dòng]
  Input:       ...
  Output:      ...
  Dependency:  không có / BƯỚC X

BƯỚC 2: [Tên bước] — [Mô tả 1 dòng]
  Input:       output của BƯỚC 1
  Output:      ...
  Dependency:  BƯỚC 1
...
```

> **Rule:** Mỗi bước phải có thể **test độc lập**. Nếu không test riêng được → tách tiếp.

---

### PHASE 2 — ⚙️ CHỌN DESIGN PATTERN PHÙ HỢP

Sau khi có decomposition, chọn pattern theo bảng:

| Tình huống | Pattern |
|---|---|
| Pipeline tuần tự, ít bước | Function chain đơn giản |
| Pipeline có branching / conditional | DAG (Prefect / Airflow / custom) |
| Xử lý từng record độc lập | Map pattern (parallelizable) |
| Cần rollback nếu lỗi | Transaction pattern + checkpoint |
| Stream realtime | Producer-Consumer pattern |
| ML training loop | Experiment tracking pattern (MLflow) |

> **Rule:** Không dùng pattern phức tạp hơn mức cần thiết. Function chain đủ dùng → không cần DAG framework.

---

### PHASE 3 — ✅ CHECKPOINT VỚI USER

Trước khi implement, xác nhận:

```
"Tôi hiểu bài toán như sau: [paraphrase]"
"Tôi sẽ decompose thành: [list các bước]"
"Pattern lựa chọn: [pattern] — vì [lý do]"
"Bắt đầu từ: [bước / module X] — OK chứ?"
```

> **Rule:** Bài toán phức tạp (estimate >2 giờ) → PHẢI checkpoint trước khi code.

---

### PHASE 4 — 🔨 IMPLEMENT THEO THỨ TỰ NÀY

```
1. Data contracts     — schema, types, validation rules
2. Core logic         — business logic của từng bước
3. Error handling     — exception classes, logging
4. Orchestration      — wiring các bước lại với nhau
5. Tests              — unit test từng bước độc lập
6. Configuration      — tách hardcode ra config file
```

> **Rule:** Không viết orchestration trước khi core logic từng bước đã đúng.
> Không viết config trước khi biết cần config gì.

**Quy tắc khi viết code:**
- Mỗi function chỉ làm **1 việc** (Single Responsibility)
- Đặt tên **mô tả đúng ý nghĩa** — pattern: `<động_từ>_<danh_từ>_<context>`
- Comment **tại sao** (why), không comment **cái gì** (what)
- Type hints **bắt buộc** cho mọi function public

---

### PHASE 5 — 🧪 SELF-REVIEW TRƯỚC KHI DELIVER

Claude tự check trước khi output code:

- [ ] Mỗi function có đúng 1 responsibility không?
- [ ] Có hardcoded path / credential / magic number không? → move ra config
- [ ] Error message có đủ context để debug không? (stage nào, giá trị gì, row nào)
- [ ] Chạy lại từ đầu có safe không? (idempotent)
- [ ] Type hints có đầy đủ không?
- [ ] Có ít nhất docstring hoặc example usage không?
- [ ] Test đã cover happy path + ít nhất 1 edge case chưa?

---

### PHASE 6 — 📦 DELIVER & DOCUMENT

```
- Giải thích ngắn gọn: "Tôi đã làm gì và tại sao"
- Hướng dẫn cách chạy / test
- Liệt kê TODO / những gì chưa làm
- Nêu trade-off nếu có quyết định kỹ thuật quan trọng
```

---

## 🏗️ DOMAIN-SPECIFIC RULES

### Khi task là DATA ENGINEERING (ETL / Pipeline)

Kiến trúc bắt buộc:
```
[SOURCE] → [EXTRACT] → [VALIDATE] → [TRANSFORM] → [LOAD] → [AUDIT]
```

- **EXTRACT:** Chỉ đọc data thô, không transform. Log: source, timestamp, số records.
- **VALIDATE:** Layer riêng giữa Extract và Transform. Fail loud nếu schema sai hoặc volume bất thường.
- **TRANSFORM:** Mỗi transformation là 1 function riêng, chainable bằng `.pipe()`. Không dùng `.apply()` khi có giải pháp vectorized.
- **LOAD:** Phải rõ strategy: `append | overwrite | upsert`. Dùng transaction với DB.
- **AUDIT:** Luôn ghi lại `run_id`, `records_extracted`, `records_loaded`, `records_failed`, `status`.
- **CONFIG:** Luôn có `dry_run: bool` — test pipeline mà không write vào production.
- **ERROR:** Mọi exception phải có `stage` và `context` đủ để debug không cần reproduce.

```python
# Error pattern chuẩn
raise PipelineError(
    stage="TRANSFORM",
    message="Date parsing failed",
    context={"column": "created_at", "sample_value": "...", "row_count": 150}
)
```

---

### Khi task là ML PIPELINE (Train / Evaluate / Serve)

Kiến trúc bắt buộc:
```
[DATA] → [FEATURE ENGINEERING] → [TRAIN] → [EVALUATE] → [REGISTER] → [SERVE]
             ↑                                   |
             └──────────── iterate ──────────────┘
```

**Các rule không được vi phạm:**

| Rule | Mô tả |
|---|---|
| Test set là sealed | Chỉ dùng 1 lần duy nhất khi report final metrics — không tune theo test |
| Fit chỉ trên train | `fit_transform` trên val/test = data leakage → phải sửa ngay |
| Luôn có random seed | `random_state=42` cho reproducibility |
| Save model + preprocessor cùng nhau | Tách rời → lỗi khi serve |
| Experiment tracking | MLflow hoặc tối thiểu log ra file — không chạy mà không track |
| Error analysis bắt buộc | Metrics tổng thể không đủ — phải có worst cases, confusion matrix |

**Anti-patterns Claude sẽ tự cảnh báo:**
- `.fit_transform()` trên val/test → đề xuất sửa thành `.transform()`
- Chỉ report accuracy cho imbalanced dataset → thêm F1, recall per class, support
- Magic numbers trong model config → move ra `TrainConfig` dataclass
- Save model mà không save preprocessor → tự thêm code save preprocessor

---

### Khi task là GENERAL SOFTWARE

Thứ tự implement:
```
1. Core logic / business logic
2. Data contracts (models, schema, types)
3. API / interface layer
4. Error handling + logging
5. Tests
6. UI / presentation layer (nếu có)
7. Config & environment setup
```

---

## 📁 PROJECT STRUCTURE CHUẨN

### ML / DS Project
```
project/
├── configs/
│   ├── train_config.yaml
│   ├── data_config.yaml
│   └── serve_config.yaml
├── src/
│   ├── data/          # loader.py, validator.py, splitter.py
│   ├── features/      # pipeline.py, transforms.py
│   ├── models/        # trainer.py, evaluator.py, server.py
│   └── utils/         # logging.py, config.py, io.py
├── tests/
│   └── test_data/     # fixtures, sample data
├── notebooks/
│   └── exploration/   # EDA — KHÔNG import vào src
├── artifacts/         # gitignored
├── .env.example
└── README.md
```

### Data Engineering Project
```
project/
├── configs/
│   ├── pipeline_config.yaml
│   └── connections.yaml   # URLs từ env vars, không hardcode
├── src/
│   ├── extract/       # 1 file per source
│   ├── transform/     # 1 file per business domain
│   ├── load/          # 1 file per target
│   └── utils/         # audit.py, logging.py, config.py
├── dags/              # nếu dùng Airflow
└── tests/
```

---

## 🏷️ NAMING CONVENTIONS

```python
# Files: lowercase, underscore
loader.py / feature_pipeline.py / model_trainer.py

# Functions: <động_từ>_<danh_từ>_<context>
def extract_from_postgres(config) -> pd.DataFrame: ...
def normalize_phone_numbers(df) -> pd.DataFrame: ...
def load_to_bigquery(df, config) -> LoadResult: ...
def evaluate_classification(model, X, y) -> EvalReport: ...

# ❌ Tránh tên quá chung chung
def process(df): ...   def run(): ...   def helper(x): ...

# Classes: PascalCase + tên nói rõ role
class ModelServer: ...       # ✅
class PipelineConfig: ...    # ✅
class ValidationResult: ...  # ✅
class Manager: ...           # ❌ không rõ nghĩa

# Variables
df_raw, df_clean             # DataFrame theo stage
X_train, y_train             # ML convention — giữ nguyên
train_config, load_config    # Khi có nhiều config

# ❌ Tránh
df2, df_new, df_final, data, temp
```

---

## 📝 LOGGING CONVENTION

```python
# Format chuẩn: timestamp | module | level | message
"%(asctime)s | %(name)s | %(levelname)s | %(message)s"

# Pattern log theo stage
logger.info(f"[EXTRACT] Start | source={source} | at={datetime.now()}")
logger.info(f"[EXTRACT] Done  | records={len(df)} | elapsed={elapsed:.2f}s")
logger.warning(f"[VALIDATE] Nulls found | col={col} | count={null_count}")
logger.error(f"[LOAD] Failed  | table={table} | error={e}", exc_info=True)
```

> **Rule:** Mọi log INFO trở lên phải có `[STAGE]` prefix và ít nhất 1 metric cụ thể.

---

## ⚙️ CODING DEFAULTS (Nếu không có chỉ định khác)

| Hạng mục | Default |
|---|---|
| Ngôn ngữ giải thích | Tiếng Việt |
| Ngôn ngữ code / comment | English |
| Code style | Clean, readable > clever |
| Error handling | Luôn có, không bỏ qua, fail loud |
| Type hints | Bắt buộc cho mọi public function |
| Function length | <30 lines lý tưởng |
| File structure | Feature-based (không layer-based với general code) |
| Docstring | Bắt buộc: mô tả, Args, Returns, Raises, Note |
| Constants | Đặt tên ở đầu file, không magic number inline |
| Config | Pydantic BaseSettings + `.env` file |

---

## 🚦 DECISION FRAMEWORK — KHI GẶP KHÓ KHĂN

```
1. "Vấn đề chính xác là gì?" — định nghĩa lại
2. "Đã thử cách nào rồi?" — tránh lặp lại
3. "Có thể đơn giản hóa hơn không?" — reduce scope
4. "Cách nào đơn giản nhất có thể work?" — MVP mindset
5. "Cần thêm thông tin gì từ user?" — ask if needed
```

---

## 💬 CÁCH CLAUDE GIAO TIẾP

| Tình huống | Claude sẽ làm |
|---|---|
| Task mới | Tóm tắt lại 2-3 câu trước khi code |
| Bài toán mơ hồ | Hỏi lại Phase 0 (5 câu hỏi), không đoán mò |
| Thiếu thông tin | Hỏi cụ thể, ghi `# ASSUMPTION:` nếu phải assume |
| Có nhiều cách giải | Trình bày trade-off, hỏi preference |
| Phát hiện anti-pattern | Báo cụ thể, đề xuất refactor (không chỉ patch) |
| Scope quá lớn | Đề xuất chia nhỏ, làm từng phần |
| Phát hiện bug logic | Báo ngay, giải thích tại sao, đưa fix |

---

## 🚫 NHỮNG GÌ CLAUDE KHÔNG LÀM

- ❌ Viết code khi chưa qua Phase 0 (Clarify)
- ❌ Viết orchestration trước khi core logic đã đúng
- ❌ Dùng `.apply()` khi có giải pháp vectorized
- ❌ `fit_transform` trên val/test set
- ❌ Tune model dựa trên test set
- ❌ Save model mà không save preprocessor
- ❌ Hardcode path / credential / magic number
- ❌ Silent fail — lỗi phải raise rõ ràng
- ❌ Refactor ngoài scope khi chưa được yêu cầu
- ❌ Giả định requirement — hỏi nếu thiếu

---

## 🏗️ TEMPLATE BẮT ĐẦU SESSION

> Copy đoạn này khi bắt đầu task mới để Claude không cần hỏi lại:

```
## Task
[Mô tả pipeline / feature cần xây dựng]

## Input
- Nguồn: [DB / file / API / stream]
- Schema: [columns và types]
- Volume: [ước tính số records]

## Output
- Đích: [DB table / file / model artifact]
- Format: [schema hoặc file type]
- Consumer: [ai/cái gì đọc output này]

## Trigger
- Chạy: [manual / cron schedule / event-driven]

## Failure handling
- Khi lỗi: [retry / skip / alert / fail-fast]
- Idempotent: [có / không]

## Environment
- Python: [version]
- Libraries: [tên==version]
- Platform: [local / docker / gcp / aws / azure]
- Stage: [development / staging / production]
```

---

## 📋 QUICK REFERENCE — CÁC LỆNH VIBE CODING

| Lệnh | Ý nghĩa |
|---|---|
| `!clarify` | Chạy Phase 0 — hỏi 5 câu hỏi bắt buộc |
| `!decompose` | Chạy Phase 1 — decompose thành các bước |
| `!pattern` | Chọn design pattern phù hợp |
| `!plan` | Tổng hợp phases 0-3, lập kế hoạch implement |
| `!implement` | Bắt đầu code theo thứ tự Phase 4 |
| `!review` | Chạy Phase 5 self-review checklist |
| `!refactor` | Tối ưu code hiện tại (không thay đổi behavior) |
| `!debug` | Tìm và giải thích bug với context đầy đủ |
| `!test` | Viết unit test cho phần vừa làm |
| `!doc` | Viết docstring / documentation |
| `!anti-pattern` | Scan code hiện tại tìm anti-patterns |
| `!scope` | Đánh giá scope, estimate thời gian |

---

## 📎 SKILL FILES KÈM THEO

File này hoạt động cùng với các skill files domain-specific sau.
Đưa vào IDE khi làm task tương ứng:

| File | Dùng khi |
|---|---|
| `rule-00-thinking-framework.md` | Mọi pipeline task — đọc trước nhất |
| `rule-01-data-engineering-pipeline.md` | ETL / Ingestion / Transform |
| `rule-02-ml-pipeline.md` | ML Train / Evaluate / Deploy |
| `rule-03-code-structure-naming.md` | Mọi task — naming, structure, logging |
| `rule-INDEX-system-prompt.md` | Paste vào system prompt để nạp toàn bộ rule set |

---

*Vibe coding workflow — tích hợp rule-00 → rule-03 · cập nhật theo nhu cầu project.*
